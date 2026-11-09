using System;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Security.Cryptography;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using System.Threading.Tasks;

[assembly: AssemblyTitle("HKMW4 香港繁體中文補丁安裝程式")]
[assembly: AssemblyDescription("HKMW4 0.5.1 cumulative translation checkpoint installer")]
[assembly: AssemblyVersion("0.5.1.0")]
[assembly: AssemblyFileVersion("0.5.1.0")]

internal static class Package {
    internal static string DirectoryPath;
    internal static string Sha(byte[] data) {
        using(var hash=SHA256.Create()) return BitConverter.ToString(hash.ComputeHash(data)).Replace("-","").ToLowerInvariant();
    }
    internal static void Extract() {
        byte[] data;
        using(var stream=Assembly.GetExecutingAssembly().GetManifestResourceStream("HKMW4.Package")) {
            if(stream==null) throw new InvalidDataException("Embedded package is missing.");
            using(var buffer=new MemoryStream()) { stream.CopyTo(buffer); data=buffer.ToArray(); }
        }
        // The release builder replaces this placeholder before compilation.
        if(Sha(data)!="@PACKAGE_SHA256@") throw new InvalidDataException("Embedded package hash mismatch.");
        DirectoryPath=Path.Combine(Path.GetTempPath(),"HKMW4-setup-"+Guid.NewGuid().ToString("N"));
        Directory.CreateDirectory(DirectoryPath);
        string prefix=Path.GetFullPath(DirectoryPath)+Path.DirectorySeparatorChar;
        using(var input=new MemoryStream(data)) using(var archive=new ZipArchive(input,ZipArchiveMode.Read)) {
            foreach(var entry in archive.Entries) {
                string target=Path.GetFullPath(Path.Combine(DirectoryPath,entry.FullName));
                if(!target.StartsWith(prefix,StringComparison.OrdinalIgnoreCase)) throw new InvalidDataException("Unsafe archive path.");
                if(String.IsNullOrEmpty(entry.Name)) {Directory.CreateDirectory(target);continue;}
                Directory.CreateDirectory(Path.GetDirectoryName(target));
                using(var source=entry.Open()) using(var output=File.Create(target)) source.CopyTo(output);
            }
        }
        foreach(string line in File.ReadAllLines(Path.Combine(DirectoryPath,"SHA256SUMS.txt"),Encoding.UTF8)) {
            if(String.IsNullOrWhiteSpace(line))continue;
            int split=line.IndexOf("  ",StringComparison.Ordinal);
            if(split!=64)throw new InvalidDataException("Invalid checksum list.");
            string path=Path.GetFullPath(Path.Combine(DirectoryPath,line.Substring(split+2).Replace('/',Path.DirectorySeparatorChar)));
            if(!path.StartsWith(prefix,StringComparison.OrdinalIgnoreCase)||Sha(File.ReadAllBytes(path))!=line.Substring(0,64))
                throw new InvalidDataException("Package file integrity check failed.");
        }
    }
    internal static void Cleanup() {
        if(String.IsNullOrEmpty(DirectoryPath))return;
        string root=Path.GetFullPath(Path.GetTempPath()).TrimEnd(Path.DirectorySeparatorChar)+Path.DirectorySeparatorChar;
        string current=Path.GetFullPath(DirectoryPath);
        if(current.StartsWith(root,StringComparison.OrdinalIgnoreCase)&&Path.GetFileName(current).StartsWith("HKMW4-setup-",StringComparison.Ordinal)) {
            try {Directory.Delete(current,true);}catch(IOException){}catch(UnauthorizedAccessException){}
        }
    }
    internal static string QuotePowerShell(string value) {return "'"+value.Replace("'","''")+"'";}
    internal static int Run(string mode,string gameRoot,Action<string> output) {
        string script=Path.Combine(DirectoryPath,"InstallPatch.ps1");
        string command="$ErrorActionPreference='Stop'; [Console]::OutputEncoding=New-Object Text.UTF8Encoding($false); try { & "+
            QuotePowerShell(script)+" -Mode "+QuotePowerShell(mode)+" -GameRoot "+QuotePowerShell(gameRoot)+
            "; exit 0 } catch { [Console]::Error.WriteLine($_.Exception.Message); exit 1 }";
        string encoded=Convert.ToBase64String(Encoding.Unicode.GetBytes(command));
        var start=new ProcessStartInfo(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.System),"WindowsPowerShell\\v1.0\\powershell.exe"),
            "-NoProfile -NonInteractive -ExecutionPolicy Bypass -EncodedCommand "+encoded);
        start.UseShellExecute=false;start.CreateNoWindow=true;start.WindowStyle=ProcessWindowStyle.Hidden;
        start.RedirectStandardOutput=true;start.RedirectStandardError=true;
        start.StandardOutputEncoding=Encoding.UTF8;start.StandardErrorEncoding=Encoding.UTF8;
        using(var process=new Process()) {
            process.StartInfo=start;
            process.OutputDataReceived+=(sender,e)=>{if(e.Data!=null)output(e.Data);};
            process.ErrorDataReceived+=(sender,e)=>{if(e.Data!=null)output(e.Data);};
            process.Start();process.BeginOutputReadLine();process.BeginErrorReadLine();process.WaitForExit();return process.ExitCode;
        }
    }
}

internal sealed class InstallerForm:Form {
    readonly TextBox folder=new TextBox();
    readonly TextBox log=new TextBox();
    readonly Button browse=new Button();
    readonly Button install=new Button();
    readonly Button uninstall=new Button();
    readonly Label status=new Label();
    bool busy;
    internal InstallerForm() {
        Text="HKMW4 香港繁體中文補丁 0.5.1";ClientSize=new Size(780,550);MinimumSize=new Size(796,589);
        Font=new Font("Microsoft JhengHei UI",10F);StartPosition=FormStartPosition.CenterScreen;
        var title=new Label {Text="香港繁體中文補丁 0.5.1",Font=new Font(Font.FontFamily,18F,FontStyle.Bold),AutoSize=true,Location=new Point(24,20)};
        var description=new Label {Text="目前進度版：15,304 條文字規則；全文覆核尚未完成\r\n請先退出遊戲。已有舊版時，先按「還原舊版／移除補丁」，再安裝本版。",AutoSize=true,Location=new Point(26,68)};
        var label=new Label {Text="遊戲根目錄（內有 sp26 資料夾）",AutoSize=true,Location=new Point(26,126)};
        folder.Text=@"C:\Games\Call of Duty - Modern Warfare 4";folder.SetBounds(28,154,608,30);folder.Anchor=AnchorStyles.Top|AnchorStyles.Left|AnchorStyles.Right;
        browse.Text="選擇資料夾";browse.SetBounds(648,152,108,32);browse.Anchor=AnchorStyles.Top|AnchorStyles.Right;
        browse.Click+=(sender,e)=>{using(var dialog=new FolderBrowserDialog {Description="選擇內有 sp26 資料夾的遊戲根目錄",SelectedPath=folder.Text})if(dialog.ShowDialog(this)==DialogResult.OK)folder.Text=dialog.SelectedPath;};
        install.Text="安裝 0.5.1";install.SetBounds(28,204,180,40);install.Click+=async(sender,e)=>await Execute("Install");
        uninstall.Text="還原舊版／移除補丁";uninstall.SetBounds(224,204,246,40);uninstall.Click+=async(sender,e)=>await Execute("Uninstall");
        status.Text="等待操作。安裝程式會備份原檔並檢查版本。";status.AutoSize=true;status.Location=new Point(28,260);
        log.Multiline=true;log.ReadOnly=true;log.ScrollBars=ScrollBars.Vertical;log.SetBounds(28,292,728,190);log.Anchor=AnchorStyles.Top|AnchorStyles.Bottom|AnchorStyles.Left|AnchorStyles.Right;
        var footnote=new Label {Text="非官方補丁。安裝程式不會啟動遊戲。",AutoSize=true,Location=new Point(28,506),Anchor=AnchorStyles.Left|AnchorStyles.Bottom};
        Controls.AddRange(new Control[]{title,description,label,folder,browse,install,uninstall,status,log,footnote});
        FormClosing+=(sender,e)=>{if(busy)e.Cancel=true;};
    }
    void Append(string line) {
        if(InvokeRequired){BeginInvoke(new Action<string>(Append),line);return;}
        log.AppendText(line+Environment.NewLine);
    }
    async Task Execute(string mode) {
        if(busy)return;
        string root=folder.Text.Trim();
        if(!File.Exists(Path.Combine(root,"sp26","sp26-cod.exe"))){status.Text="請選擇正確遊戲根目錄：未找到 sp26\\sp26-cod.exe";return;}
        busy=true;install.Enabled=uninstall.Enabled=browse.Enabled=folder.Enabled=false;log.Clear();
        status.Text=mode=="Install"?"正在安裝，請稍候……":"正在還原，請稍候……";
        try {
            int result=await Task.Run(()=>Package.Run(mode,root,Append));
            status.Text=result==0?(mode=="Install"?"安裝完成。請自行啟動遊戲核對。":"還原完成。"):
                "操作未完成。請查看下方原因；不同已裝版本須先還原。";
        } catch(Exception error){Append(error.Message);status.Text="操作未完成。請查看下方原因。";}
        finally {busy=false;install.Enabled=uninstall.Enabled=browse.Enabled=folder.Enabled=true;}
    }
}

internal static class Program {
    [STAThread]
    static int Main(string[] args) {
        bool verify=args.Length==1&&args[0]=="--verify-payload";
        try {
            Package.Extract();
            if(verify)return 0;
            if(args.Length!=0)throw new ArgumentException("Supported option: --verify-payload");
            Application.EnableVisualStyles();Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new InstallerForm());return 0;
        } catch(Exception error) {
            if(!verify)MessageBox.Show("安裝程式未能繼續：\r\n"+error.Message,"HKMW4",MessageBoxButtons.OK,MessageBoxIcon.Error);
            return 1;
        } finally {Package.Cleanup();}
    }
}
