# MW4 香港繁體中文補丁

非官方香港繁體中文補丁。此儲存庫以目前遊戲副本使用的「MW4」名稱識別項目；支援範圍以檔案雜湊為準。

**目前版本：0.5.1，翻譯覆核進度版。** 包含15,304條完整字串規則及178條選單優先規則。全故事英中核對尚未完成，仍有335項來源核對及23項語境問題待處理。未確認修復micro stutter。

## 下載及安裝

到 [Releases](https://github.com/Chamotrans/HKMW4-zh-HK/releases) 下載 `HKMW4-Setup-0.5.1.exe`。EXE內含全部補丁資料，安裝時毋須另行下載。

1. 完全退出遊戲。
2. 執行EXE，選擇遊戲根目錄，即內有 `sp26` 資料夾的目錄。
3. 如已安裝舊補丁，先按「還原舊版／移除補丁」，再按「安裝0.5.1」。也可先用舊版的 `Uninstall.cmd` 還原。
4. 安裝成功後，由你自行啟動遊戲核對。日後可再執行同一EXE移除補丁。

安裝程式會檢查遊戲版本、備份原檔及驗證補丁內容。遊戲語言須已設為 `tchinese`。安裝程式不會啟動遊戲或更改語言設定。已有完全相同版本時，會檢查檔案後保留現有安裝；不同版本須先還原。

支援Windows x64，需 [.NET Framework 4.8或更新版本](https://learn.microsoft.com/dotnet/framework/install/on-windows-10)及Windows PowerShell 5.1。EXE目前未作數碼簽署；可用Release附帶的 `SHA256SUMS.txt` 核對下載檔案。

## 版本限制

僅支援以下副本，不會強制套用至其他版本：

| 檔案 | SHA-256 |
| --- | --- |
| `sp26/sp26-cod.exe` | `69bca6d6e9dd874d0e9b194a300f1ddf9ffc264b1081f78e0e4e7d7ff4c0408e` |
| 原版 `sp26/bink2w64.dll` | `5ec0f57f58a90d0cdc83d77233d4b9344a34b1b720d30ebef65193cfc51bdd1c` |

補丁保留韓國漢字姓名；對白按英文覆核，採香港繁體書面語。只刪除句段末尾的句號，保留句中句號。底線及斜體受遊戲顯示限制而省略。此公開版沿用已交付的0.5.1文字及執行引擎，沒有新增翻譯。

## 還原及故障排查

再次執行EXE，選擇相同遊戲根目錄並按「還原舊版／移除補丁」。原檔備份存於 `sp26/_HKMW4-backup/`，安裝記錄為 `sp26/_HKMW4-install.json`。

若版本不符、原檔已被其他工具修改、語言不是 `tchinese`，或相關檔案仍被佔用，安裝程式會停止並顯示原因。請勿手動混用不同版本的補丁檔案。

## 公開內容

儲存庫附安裝程式、原始碼、文字替換資料及Bink載入器的57個位元組修改記錄。安裝時從使用者自己的相符原檔重建載入器。公開檔案不包含完整遊戲EXE、原版Bink DLL、關卡資源、帳戶設定或私人工作記錄。

`HKMW4-0.5.1-portable.zip` 是相同公開補丁的手動版本，不含完整Bink DLL。可在PowerShell內指定遊戲路徑：

```powershell
.\InstallPatch.ps1 -Mode Install -GameRoot 'D:\Games\你的遊戲'
.\InstallPatch.ps1 -Mode Uninstall -GameRoot 'D:\Games\你的遊戲'
```

## 本機建置

在Windows x64的PowerShell內執行：

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\build.ps1
python .\tests\verify_public.py
```

產物位於 `dist/`。建置會驗證EXE內嵌資料，不會顯示安裝視窗或啟動遊戲。`installer/Setup.cs` 是安裝介面；`installer/InstallPatch.ps1` 重建載入器；`installer/InstallCore.ps1` 負責備份、安裝及還原。

自訂執行DLL原始碼位於 `src/hkmw4.c`。`src/build-dll.ps1` 使用Zig 0.14.1重建DLL；普通安裝程式建置直接採用 `data/payload/hkmw4.dll`，不會重新編譯或更改執行引擎。

## 驗證狀態

已進行全部15,304條規則及178條選單規則的DLL輸出核對，以及隔離安裝、重複安裝、還原、拒絕損壞補丁及不相符版本的測試。公開EXE另有內嵌檔案完整性檢查。遊戲畫面及實機效能未在本次發佈流程測試。

本項目與遊戲發行商無關。遊戲、Bink及相關商標的權利屬各自權利人；本儲存庫不授予原遊戲或第三方軟件的權利。
