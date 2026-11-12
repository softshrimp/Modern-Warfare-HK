"""Rebuild README statistics from the shipped original/HK text pairs.

Requires matplotlib for the two PNG charts. Run from any working directory:
    python -m pip install matplotlib
    python tools/build_readme_stats.py
"""

from __future__ import annotations

import csv
import hashlib
import io
import json
from pathlib import Path
import struct
import unicodedata


ROOT = Path(__file__).resolve().parents[1]
BG = "#111310"
PANEL = "#1d211a"
WHITE = "#f1f3ec"
MUTED = "#b4bcad"
YELLOW = "#f0da35"
TEAL = "#7ec7b1"
GREY = "#626b5b"


def read_json(path: Path):
    return json.loads(path.read_text(encoding="utf-8"))


def text_sha256(path: Path) -> str:
    # Match Git's LF text blobs across Windows and Unix checkouts.
    return hashlib.sha256(path.read_bytes().replace(b"\r\n", b"\n")).hexdigest()


def fnv1a64(data: bytes) -> int:
    result = 0xCBF29CE484222325
    for byte in data:
        result = ((result ^ byte) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return result


def read_pairs(path: Path) -> tuple[bytes, list[dict]]:
    data = path.read_bytes()
    assert data[:8] == b"HKMX0001", "Unexpected text-table format"
    count = struct.unpack_from("<I", data, 8)[0]
    offset = 12
    rows = []
    for _ in range(count):
        source_hash, old_size, new_size = struct.unpack_from("<QII", data, offset)
        offset += 16
        old_bytes = data[offset:offset + old_size]
        offset += old_size
        assert data[offset] == 0, "Missing source terminator"
        offset += 1
        new_bytes = data[offset:offset + new_size]
        offset += new_size
        assert data[offset] == 0, "Missing target terminator"
        offset += 1
        assert fnv1a64(old_bytes) == source_hash, "Source hash mismatch"
        rows.append({
            "source_hash": f"{source_hash:016x}",
            "old": old_bytes.decode("utf-8"),
            "new": new_bytes.decode("utf-8"),
        })
    assert offset == len(data), "Unparsed text-table bytes"
    assert len({row["old"] for row in rows}) == count, "Repeated source key"
    assert len({row["source_hash"] for row in rows}) == count, "Hash collision"
    return data, rows


def without_punctuation_or_space(text: str) -> str:
    # Keep letters, numbers, symbols, and game control codes for this comparison.
    return "".join(
        char for char in text
        if not char.isspace() and not unicodedata.category(char).startswith(("P", "Z"))
    )


def classify(old: str, new: str) -> str:
    if old == new:
        return "unchanged"
    if without_punctuation_or_space(old) == without_punctuation_or_space(new):
        return "punctuation_spacing_only"
    return "content_changed"


def collect_statistics() -> tuple[dict, str]:
    manifest = read_json(ROOT / "data/manifest.json")
    version = manifest["version"]
    payload_path = ROOT / "data/payload/hkmw4-mission-review.bin"
    payload, rows = read_pairs(payload_path)
    payload_sha = hashlib.sha256(payload).hexdigest()
    assert payload_sha == manifest["payload"][payload_path.name]["sha256"]
    assert len(rows) == manifest["full_text_rules"]
    lexicon_path = ROOT / "data/statistics-lexicon.json"
    lexicon = read_json(lexicon_path)
    semantic_path = ROOT / f"docs/semantic-corrections-{version}.json"
    semantic_data = read_json(semantic_path)
    examples = semantic_data["examples"]
    assert semantic_data["version"] == version
    assert len(examples) == semantic_data["example_count"]
    assert len({item["original_tc"] for item in examples}) == len(examples)
    by_old = {row["old"]: row for row in rows}
    example_by_hash = {}
    for example in examples:
        row = by_old[example["original_tc"]]
        assert row["new"] == example["hk"], f"Stale example {example['number']}"
        assert row["source_hash"] == example["source_hash_fnv1a64"]
        example_by_hash[row["source_hash"]] = str(example["number"])

    terms = []
    for group in ("vocabulary", "orthography"):
        for index, (old_term, new_term) in enumerate(lexicon[group], start=1):
            assert old_term != new_term
            terms.append({
                "id": f"{group}-{index:02}", "group": group,
                "original": old_term, "hk": new_term, "strings": 0,
            })
    assert len({(term["original"], term["hk"]) for term in terms}) == len(terms)
    groups = {"vocabulary": set(), "orthography": set()}
    counts = {"unchanged": 0, "punctuation_spacing_only": 0, "content_changed": 0}
    ledger = io.StringIO(newline="")
    writer = csv.writer(ledger, lineterminator="\n")
    writer.writerow(["source_hash_fnv1a64", "change_class", "semantic_example", "hk_term_pair_ids"])
    for row in rows:
        change_class = classify(row["old"], row["new"])
        counts[change_class] += 1
        matched = []
        for term in terms:
            old, new = term["original"], term["hk"]
            # Net decrease/increase prevents retained terms from being counted.
            if (row["old"].count(old) > row["new"].count(old)
                    and row["new"].count(new) > row["old"].count(new)):
                term["strings"] += 1
                groups[term["group"]].add(row["source_hash"])
                matched.append(term["id"])
        writer.writerow([
            row["source_hash"], change_class,
            example_by_hash.get(row["source_hash"], ""), ";".join(matched),
        ])

    total = len(rows)
    union = groups["vocabulary"] | groups["orthography"]
    assert sum(counts.values()) == total
    assert len(union) <= counts["content_changed"]
    assert len(examples) <= counts["content_changed"]
    stats = {
        "version": version,
        "unit": "distinct original text key in the shipped full-text table",
        "inputs": {
            "payload": "data/payload/hkmw4-mission-review.bin",
            "payload_sha256": payload_sha,
            "semantic_examples": semantic_path.relative_to(ROOT).as_posix(),
            "semantic_examples_sha256": text_sha256(semantic_path),
            "lexicon": lexicon_path.relative_to(ROOT).as_posix(),
            "lexicon_sha256": text_sha256(lexicon_path),
        },
        "total_strings": total,
        "changed_strings": total - counts["unchanged"],
        "change_classes": counts,
        "semantic_correction_examples": len(examples),
        "localisation": {
            "matched_strings": len(union),
            "vocabulary_strings": len(groups["vocabulary"]),
            "orthography_strings": len(groups["orthography"]),
            "both_groups_strings": len(groups["vocabulary"] & groups["orthography"]),
            "pair_count": len(terms),
            "matched_pair_count": sum(term["strings"] > 0 for term in terms),
            "terms": terms,
        },
        "method": {
            "text_input_hashes": "SHA256 of UTF-8 text with CRLF normalized to LF, matching Git text blobs.",
            "change_classes": "Exact equality first; otherwise remove Unicode punctuation (P*), separators (Z*) and whitespace to identify punctuation/spacing-only changes. Remaining differences are content changes.",
            "semantic_correction_examples": "Distinct source keys in the published English/original/HK semantic-correction examples, verified against the shipped targets.",
            "localisation": "For each published pair, count each source key once when the original term decreases and the HK term increases. Deduplicate source keys across all pairs for the headline total.",
            "menu_rules": "178 priority-menu rules use a separate lookup path and are not added to the full-text denominator.",
            "overlap": "Semantic corrections and localisation may occur in the same source key. Regional term bars may overlap each other.",
        },
    }
    return stats, ledger.getvalue()


def chart_setup():
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    from matplotlib import font_manager
    from matplotlib.patches import FancyBboxPatch

    candidates = [
        Path("C:/Windows/Fonts/msjh.ttc"),
        Path("/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc"),
        Path("/System/Library/Fonts/PingFang.ttc"),
    ]
    font = next((path for path in candidates if path.is_file()), None)
    if font is None:
        raise RuntimeError("Install Microsoft JhengHei or Noto Sans CJK for Chinese charts")
    font_manager.fontManager.addfont(str(font))
    family = font_manager.FontProperties(fname=str(font)).get_name()
    plt.rcParams.update({
        "font.family": family, "text.color": WHITE, "axes.labelcolor": MUTED,
        "xtick.color": MUTED, "ytick.color": WHITE, "axes.unicode_minus": False,
        "figure.facecolor": BG, "savefig.facecolor": BG,
    })
    return plt, FancyBboxPatch


def render_overview(stats: dict, plt, box) -> None:
    fig = plt.figure(figsize=(12, 7.5), dpi=160)
    ax = fig.add_axes([0, 0, 1, 1])
    ax.set(xlim=(0, 1200), ylim=(750, 0))
    ax.axis("off")

    def label(x, y, text, size=13, color=WHITE, weight="normal", **kwargs):
        ax.text(x, y, text, fontsize=size, color=color, weight=weight, va="center", **kwargs)

    label(55, 58, f"{stats['version']}  /  翻譯包修訂統計", 24, weight="bold")
    label(55, 101, "逐條比對遊戲原有繁中與本翻譯包；單位：不同原文字串", 12, MUTED)
    cards = [
        (55, stats["total_strings"], "比對字串", WHITE),
        (425, stats["changed_strings"], "有改動", YELLOW),
        (795, stats["change_classes"]["unchanged"], "完全保留", MUTED),
    ]
    for x, value, title, color in cards:
        ax.add_patch(box((x, 146), 350, 130, boxstyle="round,pad=0,rounding_size=12",
                         facecolor=PANEL, edgecolor="none"))
        label(x + 24, 184, title, 13, MUTED)
        label(x + 24, 235, f"{value:,}", 34, color, "bold")

    label(55, 323, "改動與保留", 17, weight="bold")
    items = [
        ("content_changed", "字句／字形等內容有改動", YELLOW),
        ("punctuation_spacing_only", "只改標點或空白", TEAL),
        ("unchanged", "完全保留", GREY),
    ]
    x = 55
    for key, _, color in items:
        value = stats["change_classes"][key]
        width = value / stats["total_strings"] * 1090
        ax.add_patch(box((x, 357), width, 45, boxstyle="square,pad=0", facecolor=color, edgecolor="none"))
        x += width
    for x, (key, title, color) in zip([55, 425, 795], items):
        value = stats["change_classes"][key]
        ax.scatter([x + 5], [435], s=50, color=color, marker="s")
        label(x + 22, 435, title, 12)
        label(x, 479, f"{value:,}  /  {value / stats['total_strings']:.2%}", 21,
              MUTED if key == "unchanged" else color, "bold")

    for x, value, title, subtitle, color in [
        (55, stats["semantic_correction_examples"], "已列證的英中錯譯", "對應 README 的逐項英中例證", YELLOW),
        (610, stats["localisation"]["matched_strings"], "詞表核對的台繁轉港繁", "港式詞彙／字形合併後去重", TEAL),
    ]:
        ax.add_patch(box((x, 541), 535, 147, boxstyle="round,pad=0,rounding_size=12",
                         facecolor=PANEL, edgecolor="none"))
        label(x + 24, 576, title, 13, MUTED)
        label(x + 24, 626, f"{value:,} 條", 29, color, "bold")
        label(x + 24, 668, subtitle, 10, MUTED)
    label(55, 725, "統計範圍：完整字串表；按不同原文鍵計數", 10, MUTED)
    fig.savefig(ROOT / f"assets/statistics-{stats['version']}-overview.png", metadata={"Software": "HKMW4 statistics builder"})
    plt.close(fig)


def render_localisation(stats: dict, plt, box) -> None:
    loc = stats["localisation"]
    fig = plt.figure(figsize=(12, 8.1), dpi=160)
    canvas = fig.add_axes([0, 0, 1, 1])
    canvas.set(xlim=(0, 1200), ylim=(810, 0))
    canvas.axis("off")
    canvas.text(55, 58, "台繁轉港繁  /  詞彙與字形", fontsize=24, weight="bold", va="center")
    canvas.text(55, 102, f"{loc['pair_count']} 對詞字逐對核對；命中 {loc['matched_strings']:,} 條不同字串", fontsize=13, color=MUTED, va="center")
    canvas.text(55, 158, "港式詞彙採用次數最多的 10 對", fontsize=15, weight="bold")
    canvas.text(685, 158, "港式字形／寫法", fontsize=15, weight="bold")

    def bars(group, position, limit, color):
        terms = sorted((term for term in loc["terms"] if term["group"] == group),
                       key=lambda term: (-term["strings"], term["id"]))[:limit]
        ax = fig.add_axes(position, facecolor=BG)
        values = [term["strings"] for term in terms]
        ax.barh(range(len(terms)), values, height=0.48, color=color, zorder=3)
        ax.set_yticks(range(len(terms)), [f"{term['original']} → {term['hk']}" for term in terms], fontsize=12)
        ax.invert_yaxis()
        ax.set_xlim(0, max(values) * 1.19)
        ax.xaxis.set_major_locator(plt.MaxNLocator(4, integer=True))
        ax.grid(axis="x", color=GREY, alpha=0.25, zorder=0)
        ax.tick_params(axis="y", length=0, pad=10)
        ax.tick_params(axis="x", length=0, labelsize=10)
        ax.set_xlabel("字串數", fontsize=11, labelpad=10)
        for spine in ax.spines.values():
            spine.set_visible(False)
        for index, value in enumerate(values):
            ax.text(value + max(values) * 0.025, index, str(value), fontsize=13,
                    color=WHITE, weight="bold", va="center")

    bars("vocabulary", [0.225, 0.246, 0.265, 0.515], 10, YELLOW)
    bars("orthography", [0.748, 0.246, 0.20, 0.515], 5, TEAL)
    canvas.add_patch(box((55, 672), 1090, 91, boxstyle="round,pad=0,rounding_size=12",
                         facecolor=PANEL, edgecolor="none"))
    canvas.text(80, 704,
                f"詞彙 {loc['vocabulary_strings']:,} 條   ＋   字形／寫法 {loc['orthography_strings']:,} 條   減去重疊 {loc['both_groups_strings']:,} 條   ＝   {loc['matched_strings']:,} 條",
                fontsize=16, color=WHITE, weight="bold", va="center")
    canvas.text(80, 741, "同一字串可包含多對詞字；每對詞字的長條各自計數，合計採用去重字串數",
                fontsize=11, color=MUTED, va="center")
    fig.savefig(ROOT / f"assets/statistics-{stats['version']}-localisation.png", metadata={"Software": "HKMW4 statistics builder"})
    plt.close(fig)


def main() -> None:
    stats, ledger = collect_statistics()
    version = stats["version"]
    (ROOT / f"docs/statistics-{version}.json").write_text(
        json.dumps(stats, ensure_ascii=False, indent=2) + "\n", encoding="utf-8", newline="\n",
    )
    (ROOT / f"docs/statistics-{version}.csv").write_text(ledger, encoding="utf-8", newline="\n")
    plt, box = chart_setup()
    render_overview(stats, plt, box)
    render_localisation(stats, plt, box)
    print(json.dumps({
        "version": version, "total": stats["total_strings"],
        "changes": stats["change_classes"],
        "semantic_examples": stats["semantic_correction_examples"],
        "hk_strings": stats["localisation"]["matched_strings"],
        "vocabulary": stats["localisation"]["vocabulary_strings"],
        "orthography": stats["localisation"]["orthography_strings"],
        "overlap": stats["localisation"]["both_groups_strings"],
    }, ensure_ascii=False))


if __name__ == "__main__":
    main()
