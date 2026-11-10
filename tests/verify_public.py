"""Offline package, native table and non-executable installation fixture checks."""
from pathlib import Path
import ctypes,hashlib,json,struct,subprocess,sys,zipfile,uuid,shutil,re
R=Path(__file__).resolve().parents[1];D=R/'dist';checks=[]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def load(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def save(p,x):p.write_text(json.dumps(x,ensure_ascii=False,indent=2),encoding='utf-8')
def check(name,ok,details=None):
 assert ok,(name,details)
 checks.append(dict(check=name,result='PASS',details=details))
def snap(p):return {str(f.relative_to(p)):sha(f)for f in p.rglob('*')if f.is_file()}
def main():
 m=load(R/'data/manifest.json');version=m['version'];zpath=D/f'HKMW4-{version}-portable.zip';exe=D/f'HKMW4-Setup-{version}.exe'
 check('EXE and portable archive exist',exe.is_file()and zpath.is_file())
 verify=subprocess.run([str(exe),'--verify-payload'],timeout=30,creationflags=0x08000000)
 check('EXE embedded checksums verify without GUI',verify.returncode==0)
 with zipfile.ZipFile(zpath)as z:
  names=set(z.namelist());check('Portable archive CRC valid',z.testzip()is None)
  check('Original game binaries and private records excluded',not any(Path(n).name.lower()in ('bink2w64.dll','sp26-cod.exe','sentence-review-ledger.json')for n in names))
  check('Archive uses exact public allowlist',names=={'Install.cmd','Uninstall.cmd','InstallCore.ps1','InstallPatch.ps1','manifest.json','bink-delta.json','README.md','SHA256SUMS.txt','payload/hkmw4.dll','payload/hkmw4-changes.bin','payload/hkmw4-mission-review.bin','payload/hkmw4-sources.bin'})
  for line in z.read('SHA256SUMS.txt').decode('utf-8-sig').splitlines():
   digest,name=line.split('  ',1);assert hashlib.sha256(z.read(name)).hexdigest()==digest,name
  check('All embedded files match checksum list',True)
 for name,v in m['payload'].items():
  if name=='bink2w64.dll':continue
  p=R/'data/payload'/name;assert sha(p)==v['sha256']and p.stat().st_size==v['length'],name
 check('Four native payloads match manifest',True)
 delta=load(R/'data/bink-delta.json')
 check('Loader is distributed as 57-byte delta only',len(delta['changes'])==57 and delta['original_sha256']==m['original_bink_sha256']and delta['patched_sha256']==m['payload']['bink2w64.dll']['sha256'])
 data=(R/'data/payload/hkmw4-mission-review.bin').read_bytes();assert data[:8]==b'HKMX0001'
 count=struct.unpack_from('<I',data,8)[0];pos=12;rules=[]
 for _ in range(count):
  h,a,b=struct.unpack_from('<QII',data,pos);pos+=16
  old=data[pos:pos+a];assert data[pos+a]==0;pos+=a+1
  new=data[pos:pos+b];assert data[pos+b]==0;pos+=b+1;rules.append((h,old,new))
 check('Native rule table count and bounds valid',pos==len(data)and count==m['full_text_rules'] and count<=16000)
 lib=ctypes.WinDLL(str(R/'data/payload/hkmw4.dll'))
 init=lib.HKInitializeCandidateTest;init.argtypes=[ctypes.c_wchar_p];init.restype=ctypes.c_int
 call=lib.HKTranslateCandidateTest;call.argtypes=[ctypes.c_uint64,ctypes.c_uint64,ctypes.c_void_p,ctypes.c_uint32];call.restype=ctypes.c_void_p
 assert init(str(R/'data/payload'))
 def translate(old,i=0,fp=0):
  old=old.encode()if isinstance(old,str)else old;buffer=ctypes.create_string_buffer(old);return ctypes.string_at(call(i,fp,buffer,len(old)))
 for h,old,new in rules:assert translate(old)==new,(h,old,new)
 check('All native rules return packaged targets',True)
 integration_path=R/'data'/('integration-'+version+'.json')
 if integration_path.exists():
  integration=load(integration_path);received=integration['recovered_rules'];assert len(received)==335
  packaged={old.decode():new.decode()for _,old,new in rules}
  assert all(packaged[x['old']]==x['new']for x in received)
  check('All 335 previously omitted Gemini rules are present',True)

 data=(R/'data/payload/hkmw4-changes.bin').read_bytes();assert data[:8]==b'HKMC0002';pos=12
 count=struct.unpack_from('<I',data,8)[0]
 for _ in range(count):
  i,fp,a,b=struct.unpack_from('<QQII',data,pos);pos+=24;old=data[pos:pos+a];pos+=a+1;new=data[pos:pos+b];pos+=b+1
  assert translate(old,i,fp)==new
 check('All 178 menu priorities return packaged targets',count==178 and pos==len(data))
 for old,new in [('甲。乙。','甲。乙'),('甲。\n乙。','甲。\n乙'),('「甲。乙。」','「甲。乙」'),('甲。乙。^7','甲。乙^7'),('數字2.5，Dr. Smith。下一句。','數字2.5，Dr. Smith。下一句')]:assert translate(old)==new.encode()
 check('Internal stops, decimals and controls preserved',True)
 for old in (b'English text.',b'2.5%',b'\xff\xfe',b'\xe4\xb8'):assert translate(old)==old
 check('Unrelated non-Chinese and invalid UTF8 unchanged',True)
 # Construct wholly synthetic original game/loader files. None are executable.
 work=R/'.build'/('verify-'+uuid.uuid4().hex);work.mkdir(parents=True)
 def fixture(name):
  p=work/name/'package';p.mkdir(parents=True)
  with zipfile.ZipFile(zpath)as z:z.extractall(p)
  game=work/name/"fixture 遊戲's folder";target=game/'sp26';(target/'steam_settings').mkdir(parents=True)
  (target/'sp26-cod.exe').write_bytes(b'Non-executable HKMW4 installer test fixture.\n')
  original=b'Non-executable Bink test fixture.\n'+bytes(range(256))*4;patched=bytearray(original)
  changes=[]
  for j in (7,91,259):
   changes.append(dict(offset=j,before=original[j],after=original[j]^42));patched[j]^=42
  (target/'bink2w64.dll').write_bytes(original)
  (target/'steam_settings/configs.user.ini').write_bytes(b'[user::general]\r\nlanguage=tchinese\r\n')
  fm=load(p/'manifest.json');fm['supported_exe_sha256']=sha(target/'sp26-cod.exe');fm['original_bink_sha256']=hashlib.sha256(original).hexdigest()
  fm['payload']['bink2w64.dll']=dict(sha256=hashlib.sha256(patched).hexdigest(),length=len(patched));save(p/'manifest.json',fm)
  save(p/'bink-delta.json',dict(original_length=len(original),changes=changes));return p,game,target,fm
 def run(p,game,mode):
  assert work.resolve()in game.resolve().parents
  return subprocess.run([shutil.which('powershell.exe'),'-NoProfile','-NonInteractive','-ExecutionPolicy','Bypass','-File',str(p/'InstallPatch.ps1'),'-Mode',mode,'-GameRoot',str(game)],capture_output=True,timeout=30,creationflags=0x08000000)
 p,game,target,fm=fixture('roundtrip');before=snap(target)
 result=run(p,game,'Install');check('Delta-wrapper installation succeeds on Unicode/apostrophe path',result.returncode==0,result.stderr.decode(errors='replace')if result.returncode else None)
 check('Installed fixture payloads exactly match manifest',all(sha(target/n)==v['sha256']for n,v in fm['payload'].items()))
 receipt=load(target/'_HKMW4-install.json');check('Receipt and original backup valid',receipt['version']==version and sha(Path(receipt['backupRoot'])/'bink2w64.dll')==fm['original_bink_sha256'])
 before_repeat=snap(target);result=run(p,game,'Install');check('Repeated installation makes no changes',result.returncode==0 and snap(target)==before_repeat)
 result=run(p,game,'Uninstall');check('Uninstall restores original and removes all installed helpers',result.returncode==0 and all(sha(target/n)==v for n,v in before.items())and all(not(target/n).exists()for n in ('hkmw4.dll','hkmw4-sources.bin','hkmw4-changes.bin','hkmw4-mission-review.bin','_HKMW4-install.json')))
 for case in ('damaged-payload','wrong-exe','wrong-loader','wrong-delta','wrong-language','different-version','stray-helper'):
  p,game,target,fm=fixture(case)
  if case=='damaged-payload':(p/'payload/hkmw4-mission-review.bin').write_bytes(b'TAMPERED')
  elif case=='wrong-exe':(target/'sp26-cod.exe').write_bytes(b'Wrong synthetic version')
  elif case=='wrong-loader':(target/'bink2w64.dll').write_bytes(b'Wrong synthetic version')
  elif case=='wrong-delta':
   dx=load(p/'bink-delta.json');dx['changes'][0]['before']^=1;save(p/'bink-delta.json',dx)
  elif case=='wrong-language':(target/'steam_settings/configs.user.ini').write_bytes(b'language=english\r\n')
  elif case=='different-version':(target/'_HKMW4-install.json').write_text('{"version":"0.5.0"}',encoding='utf-8')
  elif case=='stray-helper':(target/'hkmw4.dll').write_bytes(b'Existing file from another tool')
  before=snap(target);result=run(p,game,'Install');check('Rejects '+case+' without changing target',result.returncode!=0 and snap(target)==before)
 # Exercise the compiled EXE's actual hidden PowerShell bridge without creating a form.
 p,game,target,fm=fixture('compiled-bridge');before=snap(target)
 quote=lambda x:"'"+str(x).replace("'","''")+"'"
 bridge=work/'verify-compiled-bridge.ps1'
 bridge.write_text("""$ErrorActionPreference='Stop'
Add-Type -TypeDefinition 'public static class HKMW4TestSink { public static System.Action<string> Ignore = delegate(string text) {}; }'
$assembly=[Reflection.Assembly]::LoadFile(EXE_PATH)
$type=$assembly.GetType('Package',$true)
$flags=[Reflection.BindingFlags]'NonPublic,Static'
$type.GetField('DirectoryPath',$flags).SetValue($null,PACKAGE_PATH)
$method=$type.GetMethod('Run',$flags)
foreach($mode in @('Install','Uninstall')) {
 $result=$method.Invoke($null,@($mode,GAME_PATH,[HKMW4TestSink]::Ignore))
 if([int]$result -ne 0){throw ('Compiled bridge failed: '+$mode)}
}
""".replace('EXE_PATH',quote(exe)).replace('PACKAGE_PATH',quote(p)).replace('GAME_PATH',quote(game)),encoding='utf-8-sig')
 result=subprocess.run([shutil.which('powershell.exe'),'-NoProfile','-NonInteractive','-ExecutionPolicy','Bypass','-File',str(bridge)],capture_output=True,timeout=30,creationflags=0x08000000)
 check('Compiled EXE bridge roundtrip succeeds without a GUI',result.returncode==0 and all(sha(target/n)==h for n,h in before.items())and not(target/'_HKMW4-install.json').exists(),result.stderr.decode(errors='replace')if result.returncode else None)
 check('Public source contains no private workstation paths or credential patterns',not any(re.search(r'C:[\\/]Users[\\/]|OneDrive|y\.sunnylai|github_pat_|gh[opusr]_[A-Za-z0-9]{25,}|BEGIN (?:RSA |OPENSSH )?PRIVATE KEY',p.read_text(encoding='utf-8-sig'))for p in R.rglob('*')if p.is_file()and not any(x in p.parts for x in ('.git','.build','dist'))and p.suffix.lower()in ('.cs','.ps1','.json','.md','.c','.manifest')))
 report=dict(status='PASS',checks=checks,check_count=len(checks),native_rules=m['full_text_rules'],menu_priorities=178,fixture='Synthetic non-executable files only',live_game_started=False,desktop_used=False,live_installation_changed=False,performance_tested=False,gui_manually_tested=False,artifacts={p.name:dict(bytes=p.stat().st_size,sha256=sha(p))for p in (exe,zpath)})
 save(D/'verification.json',report);print(json.dumps(dict(status='PASS',checks=len(checks),artifacts=report['artifacts']),ensure_ascii=False))
if __name__=='__main__':main()
