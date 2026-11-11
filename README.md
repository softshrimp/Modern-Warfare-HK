# 《現代戰爭 4》（2026）繁體中文香港翻譯包

以香港繁體書面語重整選單、任務提示及故事字幕的非官方中文翻譯包，同時按英文修正原有譯文。

本版對應目前遊戲副本標示的「MW4」；支援範圍以檔案雜湊為準。

**目前版本：0.5.15，Gemini文本整合版。** 包含15,639條完整字串規則及178條選單優先規則。整合Gemini 0.5.14的445條新增改動，相對0.5.4更新438條文字，並保留先前補回的335條字串；29項語境／來源對應問題另列記錄。

## 改了甚麼？

以下保留五類修訂：英中語義錯譯詳列100例，其餘四類各列5例，共120例，全部對照0.5.15已封裝的替換規則核實。「原有繁中」指相應遊戲資源的文字；同一句英文可能有多個字幕版本，表中只指該條記錄。這些例子按各自語境處理，並非全局機械替換。表中的換行以`<br>`呈現。

### 英中語義錯譯

以下100例按問題性質分組，列出完整的英文、原有繁中、0.5.15實際替換文字及修正原因。例子包含故事字幕、任務提示及共用設定／武器說明，涉及劇情內容。部分句子同時修正香港用語及標點；此處列明的是語義上的差異。

每例均保留一條具體原文記錄；相同英文在其他字幕版本可能有不同舊譯。完整字串及覆核編號另見[100例資料檔](docs/semantic-corrections-0.5.15.json)，方便逐項比對。這是已封裝修訂的精選清單，並非全遊戲錯譯的完整統計。

#### 慣用語、肯否及說話立場

| 編號 | 英文 | 原有繁中 | 本翻譯包 | 修正原因 |
| --- | --- | --- | --- | --- |
| 1 | We improvise. | 我們就地取材。 | 我們隨機應變 | improvise在此指隨機應變，不限於取用現場材料。 |
| 2 | [Like hell you will.] | ［你看著辦吧。］ | ［你想也別想］ | Like hell you will是強烈反對；原譯卻讓對方自行決定，立場相反。 |
| 3 | Yes, Sergeant. | 沒有，中士。 | 是，中士 | Yes是肯定回答，原譯卻用了否定的「沒有」。 |
| 4 | Mind yerself. | 少管我。 | 小心點 | Mind yourself是提醒對方小心，不是叫對方少管自己。 |
| 5 | Lady, you might as well be speaking Korean. | 小姐，妳說韓語我還比較聽得懂。 | 小姐，你不如說韓語吧，反正我一樣聽不懂 | 這個比喻表示完全聽不懂，原譯卻說韓語更容易聽懂。 |
| 6 | What? Am I speaking Korean? | 怎麼了？我說的是韓語吧？ | 怎麼了？難道我說的是韓語？ | 角色反問對方為何聽不明白，不是在確認自己說的是韓語。 |
| 7 | We are ready. Every base is within range. | 很清楚，所有基地都在射程內了。 | 我們準備好了。所有基地都在射程之內 | ready指準備就緒，不是理解得很清楚。 |
| 8 | [Nothing yet. I'm still trying to figure out what’s wrong.] | ［還不用，我先找出問題是出在哪裡。］ | ［還沒有，我仍在查問題出在哪裏］ | Nothing yet表示還沒有結果，不是暫時不需要。 |
| 9 | [Oh hell yeah.] | ［喔，看到了。］ | ［太好了］ | Oh hell yeah表示興奮或贊同，沒有看見東西的意思。 |
| 10 | Don’t have to tell me twice! | 不用你說我也知道！ | 不用說第二次！ | 原文說不用重複指示，並非聲稱對方未說之前自己已知道。 |

#### 動作及任務指令

| 編號 | 英文 | 原有繁中 | 本翻譯包 | 修正原因 |
| --- | --- | --- | --- | --- |
| 11 | Prep for beaching. | 準備突破。 | 準備搶灘 | beaching指搶灘，不是突破。 |
| 12 | Light ‘em up. | 燃燒他們。 | 向他們開火 | Light them up在交戰語境是向敵人開火，不是放火燃燒他們。 |
| 13 | Finish up. We’re leaving. | 快動手，我們要走了。 | 快收尾，我們要走了 | Finish up要求完成或收尾，不是要求開始動手。 |
| 14 | We need to get to that MG! | 我們得解決掉機槍巢！ | 我們得趕到那挺機槍那裏！ | get to要求趕到機槍所在位置，不是摧毀機槍巢。 |
| 15 | [Send reinforcements to the garage!] | ［增援部隊到車庫集合！］ | ［派增援部隊到車庫！］ | 這是派遣增援部隊的命令，不是向增援部隊下集合令。 |
| 16 | Rearm and resupply. | 拿好裝備和武器。 | 補充武器彈藥和物資 | rearm和resupply要求補充武器彈藥及物資，不是拿好現有裝備。 |
| 17 | Find 'em. | 我會找到他們。 | 找出他們 | Find them是命令對方找人，不是說話者承諾自己會找到。 |
| 18 | Stand down! | 投降！ | 停止行動！ | Stand down要求停止行動，原譯直接改成了投降命令。 |
| 19 | [Private Park], you have to keep moving. | ［朴二等兵，］可別脫隊了。 | ［朴二等兵，］你得繼續前進 | keep moving要求繼續前進，並非只要求不要脫隊。 |
| 20 | [We’ll go when you’re ready.] | ［您準備好我們就出手。］ | ［您準備好，我們就走］ | go指離開或出發，原譯卻改成動手攻擊。 |

#### 人物、受詞及動作對象

| 編號 | 英文 | 原有繁中 | 本翻譯包 | 修正原因 |
| --- | --- | --- | --- | --- |
| 21 | He’s in the train car. | 敵人在車廂裡。 | 他在車廂裏 | 原文只用he指代一名人物；修訂保留指代，不額外指定他是敵人。 |
| 22 | [What you’re doing...will kill us. Needlessly. Recklessly. All this death--] | ［你的所作所為……會害死所有人。這種毫無節制的暴行，造成不必要的死亡──］ | ［你這樣做……會害死我們。既魯莽又毫無必要，死了這麼多人──］ | us指包括說話者在內的一方，不能擴大成所有人；亦補回魯莽的指責。 |
| 23 | We’re not here for them. | 我們不是來抓你們的。 | 我們不是為他們而來 | them指他們，原譯卻改成對面受話者「你們」，並加入抓捕動作。 |
| 24 | [My guy], take the launcher. | ［自己人，］拿發射器。 | ［兄弟，］拿發射器 | My guy是對人的稱呼，不是在判斷對方屬於自己人。 |
| 25 | Lieutenant West. These are my scouts. | 我是韋斯特中尉，這是我的小隊。 | 我是韋斯特中尉，這幾位是我的偵察兵 | scouts明確指偵察兵，原譯只泛稱整支小隊。 |
| 26 | [Private Park], you got the spotter scope. Get a visual on our guys. | ［朴二等兵，］用望遠鏡看是不是我們的人。 | ［朴二等兵，］你有觀測鏡，找出我們的人在哪裏 | 命令是用觀測鏡看見己方人員的位置，不是辨認對方是否友軍。 |
| 27 | Get us out of the water! | 快離開水漥！ | 快帶我們離開水裏！ | Get us out要求對方帶我們離開，原譯漏了對象，亦把水域限定成水窪。 |
| 28 | And he knows you. | 他也知道你。 | 他也認識你 | knows you指認識你，不是只知道你的存在。 |
| 29 | [I trust our team. I trust you. That’s a choice.] | ［我相信我們小隊、我相信你，這樣就夠了。］ | ［我信任我們的小隊。我信任你。這就是選擇］ | That is a choice強調信任是一項選擇，原譯改成了「這樣就夠了」。 |
| 30 | [Here!] I got grenades! | ［過來拿，］我有彈藥！ | ［來拿！］我有手榴彈！ | grenades明確指手榴彈，原譯只說泛指的彈藥。 |

#### 位置、方向及空間關係

| 編號 | 英文 | 原有繁中 | 本翻譯包 | 修正原因 |
| --- | --- | --- | --- | --- |
| 31 | They’re close... | 他們快來了…… | 他們就在附近…… | close在此表示距離近，原譯卻說對方即將到來。 |
| 32 | He’s on the street. | 他回到街上了。 | 他在街上 | on the street交代目前位置，沒有返回街上的意思。 |
| 33 | To your left. | 往左走。 | 在你左邊 | To your left指出物件或人物在你左邊，不是下令往左移動。 |
| 34 | They’re across the intersection! | 他們從路口過來了！ | 他們在路口另一邊！ | across the intersection表示在路口另一邊，不是正從路口過來。 |
| 35 | Over here. Left corner. | 這個，左邊角落。 | 這邊，左邊角落 | Over here是指示位置的「這邊」，原譯卻成了指物的「這個」。 |
| 36 | Found an entry point. East wall. | 找到進入點了，在東側圍籬。 | 找到入口了，在東面的牆上 | wall是牆，原譯卻把進入點放到圍籬。 |
| 37 | I found the way down... | 我找到入口了…… | 我找到下去的路了…… | way down是下去的路，不是未指方向的一般入口。 |
| 38 | Let’s keep pushing up. | 我們繼續上樓。 | 我們繼續推進 | push up在這裏指向前推進，原譯卻指定上樓。 |
| 39 | Targets up on the train cars! | 火車裡面有敵人！ | 車廂頂上有目標！ | up on the train cars指車廂頂上，原譯把目標放到火車裏面。 |
| 40 | Oh, right here. | 喔，是我。 | 噢，就在這裏 | right here是位置指示，不是說話者表明「是我」。 |

#### 時態、進行狀態及因果

| 編號 | 英文 | 原有繁中 | 本翻譯包 | 修正原因 |
| --- | --- | --- | --- | --- |
| 41 | It’ll force the helo away. | 那直升機遲早會離開。 | 這會迫使直升機離開 | 原文指某個行動會迫使直升機離開，原譯卻說它遲早自行離開。 |
| 42 | Copy. Mayhem-1 is on station. Engaging enemy vehicles. | 收到。混亂1已抵達指定位置，準備攻擊敵方載具。 | 收到。混亂1已抵達指定位置，正在攻擊敵方車輛 | Engaging回報正在攻擊，不是仍在準備攻擊。 |
| 43 | Luca, we’re on foot. Keep driving. | 盧卡，我們再來要用走的。你繼續開── | 盧卡，我們現在步行。你繼續開車 | we are on foot交代現在已經步行，原譯卻說稍後才開始走。 |
| 44 | There’s our guys. They’re coming across the intersection. | 這是我們的人，他們要穿越路口了。 | 我們的人在那裏，正穿過路口過來 | coming across表示正在穿過路口過來，不是準備穿越。 |
| 45 | They are sweeping through in groups. | 他們會分成幾組進行搜索。 | 他們正分組搜索 | are sweeping表示搜索已在進行，不是尚未實施的計劃。 |
| 46 | Just came out blastin’, huh, Park? | 你就想火力全開上吼，朴玄俊？ | 一衝出來就猛烈開火啊，朴玄俊？ | came out blasting描述已發生的開火行為，原譯卻改成想要這樣做。 |
| 47 | Park! Where’d you go, bud? | 朴玄俊！你要去哪，兄弟？ | 朴玄俊！你去哪裏了，兄弟？ | Where did you go問剛才去了哪裏，原譯卻問接下來要去哪裏。 |
| 48 | Park, you’re drawing its fire! | 朴玄俊，你會引來攻擊！ | 朴玄俊，你正引來它的火力！ | are drawing回報正在吸引火力，原譯卻用未來警告語氣。 |
| 49 | Copy. I’ve pulled as many forces away as I could. | 收到，我盡量讓部隊遠離那邊了。 | 收到。我已盡可能把部隊調離了 | I have pulled表示已完成調離，並交代已盡最大能力，不是仍在嘗試。 |
| 50 | That MG position’s secure. | 機槍巢防禦得很好。 | 那個機槍陣地已安全 | secure回報陣地已安全或已受控制，不是評價敵人防禦得很好。 |

#### 條件、程度及數量

| 編號 | 英文 | 原有繁中 | 本翻譯包 | 修正原因 |
| --- | --- | --- | --- | --- |
| 51 | It's the best thing we can do for 'em. | 目前也沒有其他選項了。 | 這已經是我們能為他們做的最好的事了 | best表示能做的最好選擇，原譯卻斷言沒有其他選項。 |
| 52 | The explosives are in the marked crates. | 裝炸藥的箱子上應該有記號。 | 炸藥就在有標記的箱子裏 | 原文確定炸藥在有標記的箱內，原譯把它變成箱子應該有記號的猜測。 |
| 53 | You might have to take out the sniper to get across. | 您得先解決狙擊手才能安全通過。 | 你可能得先解決狙擊手，才能過去 | might have to表示可能需要，原譯把它變成必須滿足的條件。 |
| 54 | Clock’s ticking. | 沒時間了。 | 時間不多了 | Clock is ticking表示時間正在流逝，不代表已經完全沒有時間。 |
| 55 | I want that case! | 我會拿到手提箱！ | 我要那個手提箱！ | want表達要求或意願，原譯卻成了保證會拿到。 |
| 56 | Last grenades. Make 'em count! | 最後一顆榴彈，別浪費了！ | 只剩最後幾顆榴彈了。別浪費！ | grenades是複數，原譯卻只剩一顆；修訂保留複數資訊。 |
| 57 | Second MG nest destroyed! | 另一個機槍巢已摧毀！ | 第二個機槍巢已摧毀！ | second明確指第二個，原譯只說另一個。 |
| 58 | How hard does it hit? | 它火力很強嗎？ | 它的威力有多大？ | How hard問威力有多大，原譯卻成了是否很強的肯否問題。 |
| 59 | They’re trying to slow us down! | 他們打算擋下我們！ | 他們想拖慢我們！ | slow us down是拖慢行動，不等於完全擋下。 |
| 60 | Get back when you can. We’ll put them to good use. | 盡快回來，我們要好好利用這些東西。 | 方便時就回來，我們會好好利用這些東西 | when you can表示能回來時再回來，原譯卻改成盡快返回的催促。 |

#### 刪除原文沒有的內容

| 編號 | 英文 | 原有繁中 | 本翻譯包 | 修正原因 |
| --- | --- | --- | --- | --- |
| 61 | Use the dark. | 用黑暗作掩護，安靜地做掉他們。 | 利用黑暗作掩護 | Use the dark只要求利用黑暗；原譯另加了安靜殺人的命令。 |
| 62 | That’s what you’re here for. | 所以我才讓你加入行動。 | 這就是你來這裏的原因 | 原文只說這就是對方在此的原因，沒有說是說話者親自招募他。 |
| 63 | [Just remember your training.] | ［您受過軍事訓練，您沒問題。］ | ［記住你受過的訓練］ | 原文提醒記住所受訓練，沒有另外保證「你沒問題」。 |
| 64 | Fuckin’ A, dude, nice! | 厲害，兄弟，愛死你！ | 厲害，兄弟，做得好！ | nice是稱讚做得好，原文沒有「愛死你」。 |
| 65 | Watch the van! | 別打到廂型車！ | 小心那部客貨車！ | Watch the van要求留意車輛，原文並未明說不要射中它。 |
| 66 | [Please, please don't kill me!] | ［住手！不要殺我！］ | ［求求你，求求你別殺我！］ | 原文是重複懇求別殺自己，沒有下令對方住手。 |
| 67 | [Sir, the earpiece. Take it.] | ［領導人，請戴耳機，動作快點。］ | ［長官，拿着這個耳機］ | Take it要求拿取耳機，原譯另加了戴上及加快動作的要求。 |
| 68 | [Violence is a tool. You need to use it? Then use it.] | ［暴力是種工具。需要用就得用，不能心軟。］ | ［暴力是一種工具。需要用，就用］ | 原文談需要時使用暴力，沒有「不能心軟」這項額外評語。 |
| 69 | Still got some fight in you then. | 沒想到你這老傢伙還能打。 | 看來你還有力氣反抗 | 原文說仍有反抗的力氣，沒有「你這老傢伙」的年齡稱呼。 |
| 70 | Faster, Dunn. Faster. | 快點，鄧恩，要走了。 | 快一點，鄧恩，再快一點 | 原文重複催促更快，沒有「要走了」的補充。 |

#### 補回原有意思及修正指涉

| 編號 | 英文 | 原有繁中 | 本翻譯包 | 修正原因 |
| --- | --- | --- | --- | --- |
| 71 | New guy’s low on rations. | 新來的把零食吃完了。 | 新仔的口糧不多了 | low on rations表示口糧不足，原譯卻變成零食已被吃完。 |
| 72 | [They wait for you.] | ［她們隨後就去找你了。］ | ［他們在等你］ | wait for you是正在等你，原譯卻說她們稍後會去找你。 |
| 73 | [This is what revenge tastes like!] | ［這是惹錯人的下場！］ | ［復仇的滋味就是這樣！］ | 原文明確談復仇的滋味，原譯卻泛化成惹錯人的下場。 |
| 74 | [I will.] | ［明白了。］ | ［我會的］ | I will是承諾會做，原譯卻只表示明白指示。 |
| 75 | [I- I didn't do anything wrong, I swear!] | ［我──我什麼都沒做，我發誓！］ | ［我──我沒做錯事，我發誓！］ | anything wrong是否認做錯事，不是否認自己做過任何事情。 |
| 76 | [Because if these things they say are true...] | ［如果人民是真心這麼想……］ | ［因為如果他們說的這些事都是真的……］ | true關乎所述事情是否屬實，原譯卻成了人民是否真心這樣想。 |
| 77 | [I know you’re in pain, but it’s not safe here.] | ［我知道你們很害怕，但這裡不安全。］ | ［我知道你們很痛苦，但這裏不安全］ | in pain表示正受痛苦，原譯卻說是害怕。 |
| 78 | You’re broken. | 你無法回頭了。 | 你已經垮了 | broken在此指已經垮掉，原譯卻改成無法回頭。 |
| 79 | Oh, I like that... | 喔，我吃看看…… | 噢，這個我喜歡…… | I like that是喜歡或欣賞，原譯卻變成嘗試吃東西。 |
| 80 | [Why haven’t you found these bastards?] | ［為什麼還沒抓到入侵者？］ | ［為甚麼還沒找到這些混蛋？］ | found是找到，不等於已經捕獲；修訂保留原文尋人的問題。 |

#### 軍事及技術詞彙

| 編號 | 英文 | 原有繁中 | 本翻譯包 | 修正原因 |
| --- | --- | --- | --- | --- |
| 81 | Anchor-1 is two mikes out. HLZ in sight. I say again, HLZ in sight. | 船錨1距離2英里。看見降落區了。重複一次，看見降落區了。 | 船錨1還有2分鐘抵達。已看見直升機着陸區。重複，已看見直升機着陸區 | 無線電用語two mikes在此指兩分鐘後抵達，原譯卻變成兩英里的距離。 |
| 82 | Danger close! | 危險迫近！ | 注意，炮火接近友軍！ | Danger close是火力接近友軍的警告，不是一般的危險迫近。 |
| 83 | A lightweight, no-frills suppressor that reduces muzzle report. This suppressor also reduces damage range. | 基本款的輕量型消音器，可減弱槍口回饋，也會降低傷害距離。 | 基本輕型滅聲器，可降低槍聲，但會縮短有效射程 | muzzle report指槍聲，並非槍口回饋；damage range按武器語境處理為有效射程。 |
| 84 | [Our Surface-to-Air launcher.] | ［我方地對空飛彈基地。］ | ［我方的地對空導彈發射器］ | launcher是發射器，原譯卻把它變成整座基地。 |
| 85 | Drill rounds are armor-piercing. Highly effective against vehicles. | 穿甲彈能擊穿裝甲，對付載具很有效。 | 鑽頭彈能擊穿裝甲，對付車輛很有效 | Drill rounds是鑽頭彈；armor-piercing才描述其穿甲能力，兩者不應合併成同一個名稱。 |
| 86 | Stack up at the relay station! | 到變電所外集合！ | 到中繼站門外列隊！ | relay station是中繼站，stack up是在攻堅前列隊，原譯錯成變電所外集合。 |
| 87 | Could be radiation exposure. | 可能有輻射外洩。 | 可能是受到輻射影響 | radiation exposure指受到輻射影響，原譯卻把原因判斷改成輻射外洩事件。 |
| 88 | [Cover the door!] | ［堵住門口！］ | ［守住門口！］ | Cover the door要求守住或掩護門口，不是把門堵住。 |
| 89 | Angled Grip | 直角握把 | 斜角握把 | Angled Grip是斜角握把，並非直角握把。 |
| 90 | Shading Quality | 表面品質 | 着色質素 | Shading指着色，不能只泛稱物件表面。 |

#### 整句關係及場景意思

| 編號 | 英文 | 原有繁中 | 本翻譯包 | 修正原因 |
| --- | --- | --- | --- | --- |
| 91 | It shouldn't have happened in the first place, sir. They sent us in half assed, so everyone can just keep pretending we're not at war. | 事情根本就不該變成這樣，長官。上頭完全沒準備就派我們過來……好讓大家繼續假裝世界和平。 | 事情根本就不該發生，長官。他們準備不足就派我們過來，好讓所有人繼續假裝我們沒有開戰 | 準備草率不足不等於完全沒有準備；假裝我們沒有開戰也不等於假裝全球和平。 |
| 92 | Mayhem-1, Sierra-1, we’re approaching from the north alley. Watch your fire. | 混亂1，我是S1，我們要通過北邊巷子了。不要開槍。 | 混亂1，這是S1。我們正從北面巷子接近，小心別誤擊友軍 | approaching是從北面接近；Watch your fire提醒避免誤擊，不是全面停火。 |
| 93 | Ready to rain hell. | 準備好開戰了。 | 準備好猛烈轟炸了 | rain hell在此是猛烈轟炸的形容，原譯只說準備開戰，漏了攻擊方式及強度。 |
| 94 | [You bastard! Don’t shoot at me!] | ［混帳！你敢對我開槍？］ | ［混帳！別對我開槍！］ | Don’t shoot是禁止射擊的命令，原譯卻成了反問對方是否敢開槍。 |
| 95 | They’ll send more. They always do. | 馬上會有人再來的，等著看。 | 他們會再派人來，他們每次都這樣 | They always do說明對方一貫會再派人，原譯卻加了「馬上」及「等着看」。 |
| 96 | [You fight with the enemy, you’ll die as the enemy.] | ［你選擇跟敵人為伍，就得跟敵人一起死。］ | ［你跟敵人並肩作戰，就會被當成敵人殺死］ | die as the enemy指被當成敵人殺死，原譯卻只說跟敵人一起死亡。 |
| 97 | You lost them. | 他們走掉了。 | 你跟丟他們了 | You lost them指出你跟丟了人，原譯卻只說對方自行走掉。 |
| 98 | I’m taking you in. You’re done. | 我必須帶你回去，你投降吧。 | 我要逮捕你，你完了 | taking you in是逮捕；You are done是宣告對方完了，不是請對方投降。 |
| 99 | Visual on target, John! Come get some! | 目標自己上門了，約翰！不要客氣！ | 看到目標了，約翰！放馬過來！ | Visual on target回報看到目標，不表示目標自己送上門；Come get some保留挑釁語氣。 |
| 100 | I don’t know where to go... | 是要怎麼爬上去？ | 我不知道該往哪裏走…… | 原文是不知道應該往哪裏走，沒有指定是尋找向上攀爬的方法。 |

### 台繁轉香港用語

| 英文 | 原有繁中 | 本翻譯包 | 修正重點 |
| --- | --- | --- | --- |
| Home Theater | 家庭劇院 | 家庭影院 | 香港用語：影院 |
| Particle Resolution | 粒子解析度 | 粒子解像度 | 香港用語：解像度 |
| Texture Resolution | 材質解析度 | 紋理解像度 | 香港用語：紋理、解像度 |
| Fullscreen Borderless | 全螢幕無邊框 | 無邊框全屏幕 | 香港用語：屏幕；調整介面語序 |
| Benchmark | 標竿測試 | 效能測試 | 香港介面常用的效能測試 |

### 對白語氣、語助詞及粗口

| 英文 | 原有繁中 | 本翻譯包 | 修正重點 |
| --- | --- | --- | --- |
| Speed the fuck up! | 媽的快一點！ | 快點，別磨蹭！ | 以催促語氣表達原文的不耐煩 |
| Oh fuck. | 喔幹。 | 噢，糟了 | 按驚愕語境改寫咒罵 |
| [What the fuck?!] | ［幹！搞什麼？］ | ［搞甚麼飛機？］ | 按責問語境採用「搞甚麼飛機」 |
| Oh, but maybe the new guy might like it, huh? | 喔，搞不好新來的菜鳥會喜歡喔？ | 噢，說不定新仔會喜歡？ | 以新仔取代菜鳥，調整台式語氣 |
| [Hey, what the fuck?] | ［欸，你丟哪啊？］ | ［喂，搞甚麼飛機？］ | 以喂取代欸，刪除英文沒有的丟擲動作 |

### 軍事術語、軍階及部隊名稱

| 英文 | 原有繁中 | 本翻譯包 | 修正重點 |
| --- | --- | --- | --- |
| Shadows incoming! | 影子組織過來了！ | 暗影部隊過來了！ | Shadows指暗影部隊 |
| See, that doesn’t matter, Private. Because you are going to eat it all. Right now. | 那不重要啦，菜鳥。你現在要把這些全部吃完，動作快。 | 那不重要，二等兵。你要把這些全部吃完。現在就吃 | Private是二等兵軍階，不能一律叫菜鳥或新仔 |
| A lightweight, no-frills suppressor that reduces muzzle report. This suppressor also reduces damage range. | 基本款的輕量型消音器，可減弱槍口回饋，也會降低傷害距離。 | 基本輕型滅聲器，可降低槍聲，但會縮短有效射程 | muzzle report指槍聲；damage range按武器語境譯作有效射程 |
| SAM sites are on the east side of the island. | 地對空基地在小島東邊。 | 地對空導彈陣地在小島東面 | 明確交代SAM地對空導彈陣地 |
| Got eyes on the relay station! | 看到變電所了！ | 看到中繼站了！ | relay station指中繼站 |

### 字幕標點及分句

| 英文 | 原有繁中 | 本翻譯包 | 修正重點 |
| --- | --- | --- | --- |
| Luca, get us out of the city. We’ll clear the road. | 盧卡，帶我們離開這座城市。我們會幫你開路。 | 盧卡，帶我們離開這座城市。我們會幫你開路 | 保留句中句號，只刪除句段最尾句號 |
| I didn’t leave his side. He was shot before I could react. | 我一直在他身邊。他被射中時，我根本來不及反應。 | 我一直在他身邊。他被射中時，我根本來不及反應 | 保留兩句的分隔 |
| [No visual.] | ［沒看到啊。］ | ［沒看到啊］ | 句末句號在括號內，仍只刪除該句號 |
| [Ko Jin-Hyuk, first objective complete.] | ［高鎮赫，第一個目標完成了。］ | ［高鎮赫，第一個目標完成了］ | 保留韓國漢字姓名及括號，刪除最尾句號 |
| You were killed by a grenade.<br>Watch out for the grenade danger indicator. | 你被一枚手榴彈炸死了。<br>留意手榴彈危險警示。 | 你被一枚手榴彈炸死了。<br>留意手榴彈危險警示 | 保留換行和句中句號；換行不等於另一句段 |

韓國漢字姓名保留原有寫法；new guy／newbie可譯作「新仔」，Private則按軍階譯作「二等兵」。字幕只刪除整個句段最尾的句號，保留句中句號及換行。底線及斜體因遊戲顯示限制而省略。

新版整合詳情見[0.5.15更新記錄](docs/release-0.5.15.md)；尚待佐證的[語境／來源問題](docs/context-questions-0.5.15.json)保留供後續追查。

## 下載及安裝

到 [Releases](https://github.com/softshrimp/Modern-Warfare-HK/releases) 下載 `HKMW4-Setup-0.5.15.exe`。EXE內含全部補丁資料，安裝時毋須另行下載。

1. 完全退出遊戲。
2. 執行EXE，選擇遊戲根目錄，即內有 `sp26` 資料夾的目錄。
3. 如已安裝舊補丁，先按「還原舊版／移除補丁」，再按「安裝0.5.15」。也可先用舊版的 `Uninstall.cmd` 還原。
4. 安裝成功後，由你自行啟動遊戲核對。日後可再執行同一EXE移除補丁。

安裝程式會檢查遊戲版本、備份原檔及驗證補丁內容。遊戲語言須已設為 `tchinese`。安裝程式不會啟動遊戲或更改語言設定。已有完全相同版本時，會檢查檔案後保留現有安裝；不同版本須先還原。

支援Windows x64，需 [.NET Framework 4.8或更新版本](https://learn.microsoft.com/dotnet/framework/install/on-windows-10)及Windows PowerShell 5.1。EXE目前未作數碼簽署；可用Release附帶的 `SHA256SUMS.txt` 核對下載檔案。

## 版本限制

僅支援以下副本，不會強制套用至其他版本：

| 檔案 | SHA-256 |
| --- | --- |
| `sp26/sp26-cod.exe` | `69bca6d6e9dd874d0e9b194a300f1ddf9ffc264b1081f78e0e4e7d7ff4c0408e` |
| 原版 `sp26/bink2w64.dll` | `5ec0f57f58a90d0cdc83d77233d4b9344a34b1b720d30ebef65193cfc51bdd1c` |

補丁保留韓國漢字姓名；對白按英文覆核，採香港繁體書面語。只刪除句段末尾的句號，保留句中句號。底線及斜體受遊戲顯示限制而省略。本版採用Gemini新版文本，補齊漏收字串並修正整合時發現的錯譯、用語及姓名問題；執行引擎維持原版本。

## 還原及故障排查

再次執行EXE，選擇相同遊戲根目錄並按「還原舊版／移除補丁」。原檔備份存於 `sp26/_HKMW4-backup/`，安裝記錄為 `sp26/_HKMW4-install.json`。

若版本不符、原檔已被其他工具修改、語言不是 `tchinese`，或相關檔案仍被佔用，安裝程式會停止並顯示原因。請勿手動混用不同版本的補丁檔案。

## 公開內容

儲存庫附安裝程式、原始碼、文字替換資料及Bink載入器的57個位元組修改記錄。安裝時從使用者自己的相符原檔重建載入器。公開檔案不包含完整遊戲EXE、原版Bink DLL、關卡資源、帳戶設定或私人工作記錄。

`HKMW4-0.5.15-portable.zip` 是相同公開補丁的手動版本，不含完整Bink DLL。可在PowerShell內指定遊戲路徑：

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

29項背景技術檢查通過，包括全部15,639條規則及178條選單規則的DLL輸出核對、335條累積字串、三條錯掛操作提示的回歸案例，以及隔離安裝、重複安裝、還原、拒絕損壞補丁及不相符版本的測試。公開EXE另有內嵌檔案完整性檢查。遊戲畫面及實機效能未在本次發佈流程測試。

本項目與遊戲發行商無關。遊戲、Bink及相關商標的權利屬各自權利人；本儲存庫不授予原遊戲或第三方軟件的權利。
