/* Cumulative Hong Kong written-Chinese localization. Built locally; no game process is started by this code.
 * It runs only if the user later installs the DLL and starts the game themselves.
 * Original menu rules keep source-ID/fingerprint and exact-text checks.
 * Reviewed dialogue uses complete original-text matching. Other valid Chinese
 * text receives conservative phrase and punctuation adaptation across chapters.
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stddef.h>
void *memcpy(void *,const void *,size_t);

int memcmp(const void *a,const void *b,size_t n){const unsigned char *x=a,*y=b;while(n--){if(*x!=*y)return *x-*y;x++;y++;}return 0;}
size_t strlen(const char *s){size_t n=0;while(s[n])n++;return n;}
/* Conservative global adaptation after the native decoder returns valid UTF-8.
 * Control spans and protected names are copied without alteration.
 */
#include <stdint.h>
#include <stddef.h>


#if defined(_WIN32)
#define API __declspec(dllexport)
#else
#define API
#endif

static int fallback_valid_utf8(const unsigned char *p, size_t n) {
    for (size_t i=0;i<n;) {
        unsigned c=p[i++], left=0, cp=0;
        if(c<0x80) { if(!c) return 0; continue; }
        if(c>=0xc2&&c<=0xdf) {left=1;cp=c&31;}
        else if(c>=0xe0&&c<=0xef) {left=2;cp=c&15;}
        else if(c>=0xf0&&c<=0xf4) {left=3;cp=c&7;}
        else return 0;
        unsigned count=left;if(i+left>n)return 0;
        while(left--){unsigned b=p[i++];if((b&0xc0)!=0x80)return 0;cp=(cp<<6)|(b&63);}
        if((count==1&&cp<0x80)||(count==2&&cp<0x800)||(count==3&&cp<0x10000)||cp>0x10ffff||(cp>=0xd800&&cp<=0xdfff))return 0;
    }
    return 1;
}
static int digit(unsigned char c){return c>='0'&&c<='9';}
static int alpha(unsigned char c){return (c>='a'&&c<='z')||(c>='A'&&c<='Z');}
typedef struct { const unsigned char *square, *angle, *curly_square, *caret; } TokenEnds;
static size_t token_len(const unsigned char *s,size_t n,int *bad,const TokenEnds *ends) {
    if(!n)return 0;
    if(s[0]=='['&&n>1&&s[1]=='{') {
        if(!ends->curly_square || ends->curly_square<s+2){*bad=1;return 0;}
        for(size_t i=2;i+1<n;i++)if(s[i]=='}'&&s[i+1]==']')return i+2;
        *bad=1;return 0;
    }
    if(s[0]=='[') {
        if(!ends->square || ends->square<s+1){*bad=1;return 0;}
        for(size_t i=1;i<n;i++)if(s[i]==']')return i+1;
        *bad=1;return 0;
    }
    if(s[0]=='<') {
        if(!ends->angle || ends->angle<s+1){*bad=1;return 0;}
        for(size_t i=1;i<n;i++)if(s[i]=='>')return i+1;
        *bad=1;return 0;
    }
    if(s[0]=='^') {
        if(n<2){*bad=1;return 0;}
        if(s[1]=='?'||s[1]=='H'||s[1]=='W') {
            if(!ends->caret || ends->caret<s+2){*bad=1;return 0;}
            for(size_t i=2;i<n;i++)if(s[i]=='^')return i+1;
            *bad=1;return 0;
        }
        if(alpha(s[1])||digit(s[1])||s[1]=='+'||s[1]=='/'||s[1]==','||s[1]==' ')return 2;
        *bad=1;return 0;
    }
    if(s[0]=='&'&&n>2&&s[1]=='&'&&digit(s[2])) {size_t i=3;while(i<n&&digit(s[i]))i++;return i;}
    if(s[0]=='%') {
        size_t i=1;
        while(i<n&&(digit(s[i])||s[i]=='-'||s[i]=='+'||s[i]=='#'||s[i]=='.'||s[i]=='*'||s[i]=='l'||s[i]=='h'||s[i]=='z'||s[i]=='j'||s[i]=='t'))i++;
        if(i<n&&(s[i]=='d'||s[i]=='i'||s[i]=='u'||s[i]=='o'||s[i]=='x'||s[i]=='X'||s[i]=='f'||s[i]=='F'||s[i]=='e'||s[i]=='E'||s[i]=='g'||s[i]=='G'||s[i]=='a'||s[i]=='A'||s[i]=='c'||s[i]=='s'||s[i]=='p'))return i+1;
        if(n>1&&(alpha(s[1])||s[1]=='{')){*bad=1;return 0;}
    }
    return 0;
}

/* A final sentence stop may precede closing quotation marks or whitespace.
   This constant-time classifier adds no suffix rescans or allocations. */
static size_t punctuation_tail_char(const unsigned char *s,size_t n) {
    if(!n)return 0;
    if(s[0]==' '||s[0]=='\t'||s[0]=='\r'||s[0]=='\n'||s[0]=='"'||s[0]=='\''||s[0]==')'||s[0]==']')return 1;
    if(n>=3) {
        unsigned cp=0;
        if((s[0]&0xf0)==0xe0 && (s[1]&0xc0)==0x80 && (s[2]&0xc0)==0x80)
            cp=((s[0]&15u)<<12)|((s[1]&63u)<<6)|(s[2]&63u);
        switch(cp){case 0x300d:case 0x300f:case 0xff09:case 0xff3d:case 0x3011:case 0x3009:case 0x300b:case 0x201d:case 0x2019:return 3;}
    }
    return 0;
}

typedef struct { const char *old_value,*new_value; } Phrase;
static const Phrase PHRASES[]={
    {"地對空飛彈","地對空導彈"},{"飛彈","導彈"},{"迫擊砲","迫擊炮"},
    {"榴彈砲","榴彈炮"},{"雷射","激光"},{"手電筒","電筒"},
    {"這裡","這裏"},{"那裡","那裏"},{"哪裡","哪裏"},{"裡面","裏面"},
    {"什麼","甚麼"},{"看著","看着"},{"帶著","帶着"},{"跟著","跟着"},
    {"留著","留着"},{"活著","活着"},{"拿著","拿着"},{"躲著","躲着"},
    {"想著","想着"},{"朝著","朝着"},{"著火","着火"},

    {"鍵盤與滑鼠","鍵盤及滑鼠"},{"全螢幕專屬","獨佔全屏幕"},{"作業系統","操作系統"},
    {"家庭劇院","家庭影院"},{"隨選材質串流","按需紋理串流"},{"標竿測試","效能測試"},
    {"帳號與網路","帳戶及網絡"},{"螢幕","屏幕"},{"解析度","解像度"},{"硬體","硬件"},
    {"軟體","軟件"},{"網路","網絡"},{"品質","質素"},{"材質","紋理"},{"著色","着色"},
    {"畫格","幀"},{"喇叭","揚聲器"},{"周遭","周圍"},{"部份","部分"}
};
static const char *KOREAN_SURNAMES[]={"李","金","朴","崔","鄭","姜","趙","尹","張","林","韓","吳","徐","申","權","黃","安","宋","柳","全","洪","高","文","梁","孫","白","許","南","沈","盧","河","郭","成","車","朱","禹","具","任","辛","池","嚴","蔡","元","方","孔","玄","咸","卞","廉","羅","裴","蘇"};
static const char *KOREAN_NAMES[]={"朴玄俊","權海秀","趙瀚","尹淙文","李周英","蘇在祐","蘇中士"};
/* 0 exact-source mismatch; -1 malformed/unsafe/no space; 1 unchanged; 2 changed. */
static int HKFallbackCandidate(const unsigned char *actual,size_t actual_n,const unsigned char *expected,size_t expected_n,
                            const unsigned char *text,size_t text_n,unsigned char *out,size_t cap,size_t *out_n) {
    if(!actual||!expected||!text||!out||!out_n||actual_n!=expected_n||memcmp(actual,expected,actual_n))return 0;
    if(text_n>8192||!fallback_valid_utf8(text,text_n))return -1;
    for(size_t k=0;k<sizeof(PHRASES)/sizeof(PHRASES[0]);k++){
        size_t a=strlen(PHRASES[k].old_value),b=strlen(PHRASES[k].new_value);
        if(a==text_n&&!memcmp(text,PHRASES[k].old_value,a)){
            if(b+1>cap)return -1;memcpy(out,PHRASES[k].new_value,b);out[b]=0;*out_n=b;
            return a!=b||memcmp(text,out,a)?2:1;
        }
    }
    TokenEnds ends={0};
    for(size_t k=0;k<text_n;k++) {
        if(text[k]==']') { ends.square=text+k; if(k && text[k-1]=='}') ends.curly_square=text+k-1; }
        else if(text[k]=='>') ends.angle=text+k;
        else if(text[k]=='^') ends.caret=text+k;
    }
    size_t j=0, final_stop=(size_t)-1, final_stop_count=0; int ambiguous=0;
    for(size_t i=0;i<text_n;) {
        int bad=0;size_t token=token_len(text+i,text_n-i,&bad,&ends);
        if(bad){
            final_stop=(size_t)-1;
            /* Do not reinterpret unknown syntax. Preserve the remaining line exactly. */
            size_t end=i;while(end<text_n&&text[end]!='\n')end++;
            size_t n=end-i;if(j+n>=cap)return -1;memcpy(out+j,text+i,n);j+=n;i=end;ambiguous=1;continue;
        }
        if(token){if((text[i]=='[' && (token<2 || text[i+1]!='{')) || (text[i]=='^' && token>2 && text[i+1]=='W'))final_stop=(size_t)-1;if(j+token>=cap)return -1;memcpy(out+j,text+i,token);j+=token;i+=token;continue;}
        size_t tail=punctuation_tail_char(text+i,text_n-i);
        if(tail){if(j+tail>=cap)return -1;memcpy(out+j,text+i,tail);j+=tail;i+=tail;continue;}
        if(i+3<=text_n&&text[i]==0xe3&&text[i+1]==0x80&&text[i+2]==0x82){
            if(j+3>=cap)return -1;
            if(final_stop!=(size_t)-1 && final_stop+3*final_stop_count==j)final_stop_count++;
            else {final_stop=j;final_stop_count=1;}
            memcpy(out+j,text+i,3);j+=3;i+=3;continue;
        }
        final_stop=(size_t)-1;
        size_t name_n=0;
        for(size_t k=0;k<sizeof(KOREAN_NAMES)/sizeof(KOREAN_NAMES[0]);k++){
            size_t n=strlen(KOREAN_NAMES[k]);
            if(n>name_n&&i+n<=text_n&&!memcmp(text+i,KOREAN_NAMES[k],n))name_n=n;
        }
        if(!name_n){
            for(size_t k=0;k<sizeof(KOREAN_SURNAMES)/sizeof(KOREAN_SURNAMES[0]);k++){
                size_t n=strlen(KOREAN_SURNAMES[k]);
                if(i+n+3<=text_n&&!memcmp(text+i,KOREAN_SURNAMES[k],n)){
                    size_t at=i+n;unsigned count=0;
                    while(at+3<=text_n&&text[at]>=0xe3&&text[at]<=0xe9&&(text[at+1]&0xc0)==0x80&&(text[at+2]&0xc0)==0x80&&count<2){ unsigned cp=((text[at]&15u)<<12)|((text[at+1]&63u)<<6)|(text[at+2]&63u); if(cp<0x3400||cp>0x9fff)break;at+=3;count++;}
                    if(count)name_n=at-i;
                    break;
                }
            }
        }
        if(name_n){if(j+name_n>=cap)return -1;memcpy(out+j,text+i,name_n);j+=name_n;i+=name_n;continue;}
        size_t best=0,replacement_n=0;const char *replacement=0;
        for(size_t k=0;k<sizeof(PHRASES)/sizeof(PHRASES[0]);k++){
            size_t n=strlen(PHRASES[k].old_value);
            if(n>best&&i+n<=text_n&&!memcmp(text+i,PHRASES[k].old_value,n)){
                best=n;replacement=PHRASES[k].new_value;replacement_n=strlen(replacement);
            }
        }
        if(best){if(j+replacement_n>=cap)return -1;memcpy(out+j,replacement,replacement_n);j+=replacement_n;i+=best;continue;}
        if(j+1>=cap)return -1;out[j++]=text[i++];
    }
    if(final_stop!=(size_t)-1){
        size_t removed=3*final_stop_count;
        for(size_t k=final_stop;k+removed<j;k++)out[k]=out[k+removed];
        j-=removed;
    }
    out[j]=0;*out_n=j;int changed=j!=text_n||memcmp(out,text,j);return ambiguous?(changed?4:3):(changed?2:1);
}







#define LIMIT 8192u
#define MAX_CAPTURE 30000u
#define SEEN_SIZE 65536u

typedef char *(__cdecl *DecoderFunction)(char *);
typedef struct { uint64_t id, fingerprint; uint32_t old_len, new_len; char *old_text, *new_text; } Change;
static HMODULE module_handle;
static DecoderFunction original_decoder;
static unsigned char *source_data, *change_data;
static DWORD source_bytes, change_bytes, source_count, change_count;
static Change *changes;
static HANDLE capture_file = INVALID_HANDLE_VALUE, status_file = INVALID_HANDLE_VALUE;
typedef struct { uint64_t hash; DWORD old_len,new_len; char *old_text,*new_text; } MissionRule;
typedef struct CacheEntry { struct CacheEntry *next; uint64_t hash; DWORD old_len,new_len; char data[]; } CacheEntry;
static unsigned char *mission_data;
static DWORD mission_bytes,mission_count,cache_count;
static LONG fallback_ambiguous_logged;
static size_t cache_bytes;
static MissionRule *mission_rules;
static CacheEntry *cache_buckets[4096];
static CRITICAL_SECTION cache_lock;
static CRITICAL_SECTION cache_shards[64];
static LONG cache_full;

static CRITICAL_SECTION capture_lock;
static uint64_t seen[SEEN_SIZE];
static LONG captured;
static DWORD captured_bytes;
static BOOL capture_enabled = TRUE;
static wchar_t directory[MAX_PATH];
static char line_buffer[65536];

void *memcpy(void *destination, const void *source, size_t length) {
    unsigned char *d = destination; const unsigned char *s = source;
    while (length--) *d++ = *s++;
    return destination;
}
void *memset(void *destination, int value, size_t length) {
    unsigned char *d = destination;
    while (length--) *d++ = (unsigned char)value;
    return destination;
}
static size_t wide_length(const wchar_t *value) { size_t n = 0; while (value[n]) n++; return n; }
static BOOL equal_bytes(const void *left, const void *right, size_t count) {
    const unsigned char *a = left, *b = right;
    for (size_t i = 0; i < count; i++) if (a[i] != b[i]) return FALSE;
    return TRUE;
}
static void make_path(wchar_t out[MAX_PATH], const wchar_t *name) {
    size_t n = wide_length(directory), m = wide_length(name);
    if (n + m + 2 >= MAX_PATH) { out[0] = 0; return; }
    memcpy(out, directory, n * sizeof(wchar_t));
    if (n && out[n-1] != L'\\') out[n++] = L'\\';
    memcpy(out+n, name, (m+1) * sizeof(wchar_t));
}
static uint64_t hash_bytes(const unsigned char *data, size_t size) {
    uint64_t h = UINT64_C(0xcbf29ce484222325);
    for (size_t i=0; i<size; i++) h = (h ^ data[i]) * UINT64_C(0x100000001b3);
    return h;
}
static uint32_t read32(const unsigned char *p) { uint32_t v; memcpy(&v,p,4); return v; }
static uint64_t read64(const unsigned char *p) { uint64_t v; memcpy(&v,p,8); return v; }
static void status(const char *message) {
    if (status_file == INVALID_HANDLE_VALUE) return;
    DWORD n=0, length=0; while (message[length]) length++;
    WriteFile(status_file,message,length,&n,NULL);
    WriteFile(status_file,"\r\n",2,&n,NULL);
}
static BOOL readable(const void *pointer, size_t count) {
    MEMORY_BASIC_INFORMATION information;
    uintptr_t address=(uintptr_t)pointer;
    if (!pointer || address + count < address) return FALSE;
    while (count) {
        if (!VirtualQuery((const void *)address,&information,sizeof(information))) return FALSE;
        if (information.State != MEM_COMMIT || (information.Protect & (PAGE_NOACCESS|PAGE_GUARD))) return FALSE;
        uintptr_t end=(uintptr_t)information.BaseAddress+information.RegionSize;
        if (end<=address) return FALSE;
        size_t step=(size_t)(end-address); if(step>count)step=count;
        address+=step; count-=step;
    }
    return TRUE;
}
static BOOL bounded_length(const char *text, size_t skip, DWORD *length) {
    uintptr_t base=(uintptr_t)text;
    if (!text || skip>LIMIT || base+LIMIT<base) return FALSE;
    size_t at=skip;
    while(at<=LIMIT) {
        MEMORY_BASIC_INFORMATION info;
        uintptr_t address=base+at;
        if (!VirtualQuery((void*)address,&info,sizeof(info)) || info.State!=MEM_COMMIT ||
            (info.Protect&(PAGE_NOACCESS|PAGE_GUARD))) return FALSE;
        uintptr_t end=(uintptr_t)info.BaseAddress+info.RegionSize;
        if(end<=address) return FALSE;
        size_t n=(size_t)(end-address), remaining=LIMIT+1u-at;
        if(n>remaining)n=remaining;
        for(size_t i=0;i<n;i++) if(!((const unsigned char*)address)[i]) {*length=(DWORD)(at+i);return TRUE;}
        at+=n;
    }
    return FALSE;
}
static BOOL valid_utf8(const unsigned char *text, DWORD length, BOOL *has_chinese) {
    *has_chinese=FALSE;
    for (DWORD i=0;i<length;) {
        uint32_t code=text[i++]; unsigned continuation=0;
        if(code<0x80) { if(code<32 && code!='\n' && code!='\r' && code!='\t')return FALSE; continue; }
        if(code>=0xc2 && code<=0xdf) {code&=31;continuation=1;}
        else if(code>=0xe0 && code<=0xef) {code&=15;continuation=2;}
        else if(code>=0xf0 && code<=0xf4) {code&=7;continuation=3;}
        else return FALSE;
        unsigned count=continuation;
        if(i+count>length)return FALSE;
        while(continuation--) {unsigned b=text[i++];if((b&0xc0)!=0x80)return FALSE;code=(code<<6)|(b&63);}
        if((count==1 && code<0x80)||(count==2 && code<0x800)||(count==3 && code<0x10000))return FALSE;
        if(code>0x10ffff || (code>=0xd800 && code<=0xdfff))return FALSE;
        if((code>=0x3400 && code<=0x9fff)||(code>=0x20000 && code<=0x3134f))*has_chinese=TRUE;
    }
    return TRUE;
}
static unsigned char *read_file(const wchar_t *name, DWORD max_size, DWORD *size) {
    wchar_t path[MAX_PATH]; make_path(path,name); if(!path[0])return NULL;
    HANDLE file=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(file==INVALID_HANDLE_VALUE)return NULL;
    LARGE_INTEGER length; DWORD got=0;
    if(!GetFileSizeEx(file,&length)||length.QuadPart<12||length.QuadPart>max_size){CloseHandle(file);return NULL;}
    unsigned char *data=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(SIZE_T)length.QuadPart+1);
    BOOL ok=data && ReadFile(file,data,(DWORD)length.QuadPart,&got,NULL) && got==length.QuadPart;
    CloseHandle(file);
    if(!ok){if(data)HeapFree(GetProcessHeap(),0,data);return NULL;}
    *size=got;return data;
}
static BOOL load_mission_rules(void) {
    mission_data=read_file(L"hkmw4-mission-review.bin",4*1024*1024,&mission_bytes);
    if(!mission_data||!equal_bytes(mission_data,"HKMX0001",8))return FALSE;
    mission_count=read32(mission_data+8);if(mission_count>16000)return FALSE;
    mission_rules=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(MissionRule)*(mission_count+1));
    if(!mission_rules)return FALSE;
    size_t cursor=12;
    for(DWORD i=0;i<mission_count;i++){
        if(cursor+16>mission_bytes)return FALSE;
        MissionRule *m=mission_rules+i;m->hash=read64(mission_data+cursor);cursor+=8;
        m->old_len=read32(mission_data+cursor);cursor+=4;m->new_len=read32(mission_data+cursor);cursor+=4;
        if(m->old_len>LIMIT||m->new_len>LIMIT||cursor+(size_t)m->old_len+m->new_len+2>mission_bytes)return FALSE;
        m->old_text=(char*)mission_data+cursor;cursor+=m->old_len+1;m->new_text=(char*)mission_data+cursor;cursor+=m->new_len+1;
        BOOL chinese;if(m->old_text[m->old_len]||m->new_text[m->new_len]||!valid_utf8((unsigned char*)m->old_text,m->old_len,&chinese)||!valid_utf8((unsigned char*)m->new_text,m->new_len,&chinese))return FALSE;
        if(m->hash!=hash_bytes((unsigned char*)m->old_text,m->old_len))return FALSE;
        if(i&&mission_rules[i-1].hash>m->hash)return FALSE;
    }
    return cursor==mission_bytes;
}
static BOOL load_tables(void) {
    source_data=read_file(L"hkmw4-sources.bin",8*1024*1024,&source_bytes);
    change_data=read_file(L"hkmw4-changes.bin",1024*1024,&change_bytes);
    if(!source_data || !change_data)return FALSE;
    if(!equal_bytes(source_data,"HKMS0001",8)||!equal_bytes(change_data,"HKMC0002",8))return FALSE;
    source_count=read32(source_data+8); change_count=read32(change_data+8);
    if(source_count>100000 || source_bytes != 12u+source_count*20u || change_count>2000)return FALSE;
    for(DWORD i=1;i<source_count;i++)if(read64(source_data+12+(i-1)*20)>=read64(source_data+12+i*20))return FALSE;
    changes=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(Change)*(change_count+1));
    if(!changes)return FALSE;
    size_t cursor=12;
    for(DWORD i=0;i<change_count;i++) {
        if(cursor+24>change_bytes)return FALSE;
        changes[i].id=read64(change_data+cursor);cursor+=8;
        changes[i].fingerprint=read64(change_data+cursor);cursor+=8;
        if((changes[i].id==0)==(changes[i].fingerprint==0))return FALSE;
        changes[i].old_len=read32(change_data+cursor);cursor+=4;
        changes[i].new_len=read32(change_data+cursor);cursor+=4;
        DWORD a=changes[i].old_len,b=changes[i].new_len;
        if(a>LIMIT||b>LIMIT||cursor+(size_t)a+b+2>change_bytes)return FALSE;
        changes[i].old_text=(char *)change_data+cursor;cursor+=a+1;
        changes[i].new_text=(char *)change_data+cursor;cursor+=b+1;
        if(changes[i].old_text[a] || changes[i].new_text[b])return FALSE;
        BOOL chinese;
        if(!valid_utf8((unsigned char*)changes[i].old_text,a,&chinese)||!valid_utf8((unsigned char*)changes[i].new_text,b,&chinese))return FALSE;
        for(DWORD j=0;j<i;j++) {
            if(changes[i].id && changes[j].id==changes[i].id)return FALSE;
            if(changes[i].fingerprint && changes[j].fingerprint==changes[i].fingerprint)return FALSE;
        }
    }
    return cursor==change_bytes;
}
static uint64_t source_id(uint64_t fingerprint, DWORD length) {
    DWORD lo=0,hi=source_count;
    while(lo<hi){DWORD mid=lo+(hi-lo)/2;uint64_t h=read64(source_data+12+mid*20);if(h<fingerprint)lo=mid+1;else hi=mid;}
    if(lo==source_count)return 0;
    const unsigned char *row=source_data+12+lo*20;
    return read64(row)==fingerprint && read32(row+16)==length ? read64(row+8) : 0;
}
static const char *translate(uint64_t id, uint64_t fingerprint, const char *text, DWORD length) {
    for(DWORD i=0;i<change_count;i++) {
        Change *change=changes+i;
        BOOL selected=(change->id && change->id==id) || (change->fingerprint && change->fingerprint==fingerprint);
        if(selected && length==change->old_len && equal_bytes(text,change->old_text,length))return change->new_text;
    }
    return text;
}
static const char *general_translate(const char *text,DWORD length,BOOL chinese) {
    uint64_t h=hash_bytes((const unsigned char*)text,length);DWORD lo=0,hi=mission_count;
    while(lo<hi){DWORD mid=lo+(hi-lo)/2;if(mission_rules[mid].hash<h)lo=mid+1;else hi=mid;}
    while(lo<mission_count&&mission_rules[lo].hash==h){MissionRule *m=mission_rules+lo++;
        if(m->old_len==length&&equal_bytes(text,m->old_text,length))return m->new_text;}
    if(!chinese)return text;
    size_t bucket=(size_t)(h&4095u), shard=bucket&63u;
    EnterCriticalSection(&cache_shards[shard]);
    for(CacheEntry *e=cache_buckets[bucket];e;e=e->next)
        if(e->hash==h&&e->old_len==length&&equal_bytes(e->data,text,length)) {
            const char *p=e->new_len==0xffffffffu?text:e->data+length+1;
            LeaveCriticalSection(&cache_shards[shard]);return p;
        }
    LeaveCriticalSection(&cache_shards[shard]);
    if(InterlockedCompareExchange(&cache_full,0,0))return text;
    unsigned char out[LIMIT+1];size_t out_len=0;
    int result=HKFallbackCandidate((unsigned char*)text,length,(unsigned char*)text,length,(unsigned char*)text,length,out,sizeof(out),&out_len);
    BOOL changed=(result==2||result==4);
    if(result<1||result>4)return text;
    size_t size=sizeof(CacheEntry)+(size_t)length+1+(changed?out_len+1:0);
    EnterCriticalSection(&cache_shards[shard]);
    for(CacheEntry *e=cache_buckets[bucket];e;e=e->next)
        if(e->hash==h&&e->old_len==length&&equal_bytes(e->data,text,length)) {
            const char *p=e->new_len==0xffffffffu?text:e->data+length+1;
            LeaveCriticalSection(&cache_shards[shard]);return p;
        }
    EnterCriticalSection(&cache_lock);
    if(cache_count>=65536 || size>128u*1024u*1024u-cache_bytes) {
        InterlockedExchange(&cache_full,1);LeaveCriticalSection(&cache_lock);LeaveCriticalSection(&cache_shards[shard]);return text;
    }
    CacheEntry *entry=HeapAlloc(GetProcessHeap(),0,size);
    if(!entry){InterlockedExchange(&cache_full,1);LeaveCriticalSection(&cache_lock);LeaveCriticalSection(&cache_shards[shard]);return text;}
    cache_count++;cache_bytes+=size;LeaveCriticalSection(&cache_lock);
    entry->hash=h;entry->old_len=length;entry->new_len=changed?(DWORD)out_len:0xffffffffu;
    memcpy(entry->data,text,length);entry->data[length]=0;
    if(changed)memcpy(entry->data+length+1,out,out_len+1);
    entry->next=cache_buckets[bucket];cache_buckets[bucket]=entry;
    const char *answer=changed?entry->data+length+1:text;LeaveCriticalSection(&cache_shards[shard]);return answer;
}
static size_t append_hex(char *out, uint64_t value) {
    static const char digits[]="0123456789abcdef";
    for(int i=15;i>=0;i--){out[i]=digits[value&15];value>>=4;}
    return 16;
}
static size_t append_json(char *out, const char *text, DWORD length) {
    static const char digits[]="0123456789abcdef";
    size_t n=0;
    for(DWORD i=0;i<length;i++) {
        unsigned char c=(unsigned char)text[i];
        if(c=='"'||c=='\\'){out[n++]='\\';out[n++]=(char)c;}
        else if(c=='\n'){out[n++]='\\';out[n++]='n';}
        else if(c=='\r'){out[n++]='\\';out[n++]='r';}
        else if(c=='\t'){out[n++]='\\';out[n++]='t';}
        else if(c<32){out[n++]='\\';out[n++]='u';out[n++]='0';out[n++]='0';out[n++]=digits[c>>4];out[n++]=digits[c&15];}
        else out[n++]=(char)c;
    }
    return n;
}
static void capture(uint64_t id,uint64_t fingerprint,const char *text,DWORD length,BOOL patched) {
    if(capture_file==INVALID_HANDLE_VALUE)return;
    uint64_t key=hash_bytes((const unsigned char*)text,length)^id;
    if(!key)key=1;
    EnterCriticalSection(&capture_lock);
    if(!capture_enabled || captured>=MAX_CAPTURE || captured_bytes>=16u*1024u*1024u){LeaveCriticalSection(&capture_lock);return;}
    size_t slot=(size_t)key&(SEEN_SIZE-1);
    while(seen[slot] && seen[slot]!=key)slot=(slot+1)&(SEEN_SIZE-1);
    if(seen[slot]==key){LeaveCriticalSection(&capture_lock);return;}
    seen[slot]=key;
    char *out=line_buffer;
    const char prefix[]="{\"id\":\"";memcpy(out,prefix,sizeof(prefix)-1);out+=sizeof(prefix)-1;
    out+=append_hex(out,id);
    const char middle[]="\",\"input_hash\":\"";memcpy(out,middle,sizeof(middle)-1);out+=sizeof(middle)-1;
    out+=append_hex(out,fingerprint);
    const char value[]="\",\"text\":\"";memcpy(out,value,sizeof(value)-1);out+=sizeof(value)-1;
    out+=append_json(out,text,length);
    const char *end=patched?"\",\"patched\":true}\r\n":"\",\"patched\":false}\r\n";
    while(*end)*out++=*end++;
    DWORD length_bytes=(DWORD)(out-line_buffer),written=0;
    if(length_bytes<=16u*1024u*1024u-captured_bytes) {
        if(WriteFile(capture_file,line_buffer,length_bytes,&written,NULL) && written==length_bytes) {
            captured_bytes+=length_bytes;captured++;
        } else capture_enabled=FALSE;
    }
    LeaveCriticalSection(&capture_lock);
}
static char *__cdecl decode_hook(char *encoded) {
    uint64_t fingerprint=0,id=0;DWORD raw_length=0;
    if(readable(encoded,3) && (((unsigned char)encoded[0]&0xc0)==0x80) && bounded_length(encoded,3,&raw_length)) {
        fingerprint=hash_bytes((const unsigned char*)encoded,raw_length);
        id=source_id(fingerprint,raw_length);
    }
    char *text=original_decoder(encoded);
    if(!text)return text;
    DWORD length;BOOL chinese;
    if(!bounded_length(text,0,&length)||!valid_utf8((unsigned char*)text,length,&chinese))return text;
    const char *result=translate(id,fingerprint,text,length);
    if(result==text)result=general_translate(text,length,chinese);
    if(chinese && fingerprint)capture(id,fingerprint,text,length,result!=text);
    return (char *)result;
}
static void write_jump(unsigned char *where,const void *destination) {
    where[0]=0xff;where[1]=0x25;where[2]=where[3]=where[4]=where[5]=0;
    memcpy(where+6,&destination,8);
}
static BOOL install_hook(void) {
    static const unsigned char signature[]={0x41,0x57,0x41,0x56,0x41,0x55,0x41,0x54,0x56,0x57,0x55,0x53,0x48,0x83,0xec,0x28,0x48,0x89,0xce,0x80,0x39,0xbf,0x0f,0x8f};
    unsigned char *base=(unsigned char*)GetModuleHandleW(NULL),*found=NULL;
    IMAGE_DOS_HEADER *dos=(IMAGE_DOS_HEADER*)base;
    if(dos->e_magic!=IMAGE_DOS_SIGNATURE)return FALSE;
    IMAGE_NT_HEADERS64 *nt=(IMAGE_NT_HEADERS64*)(base+dos->e_lfanew);
    if(nt->Signature!=IMAGE_NT_SIGNATURE || nt->FileHeader.Machine!=IMAGE_FILE_MACHINE_AMD64)return FALSE;
    /* This pilot supports one exact executable hash, checked by its installer.
       The copied image has one full signature at RVA 0xF0CEF0. There is no
       version fallback and no scan of unrelated memory. */
    if(nt->OptionalHeader.SizeOfImage!=0x15948000u)return FALSE;
    found=base+0xF0CEF0u;
    if(!readable(found,sizeof(signature)) || !equal_bytes(found,signature,sizeof(signature))) {
        status("decoder_signature_mismatch: no runtime patch applied");return FALSE;
    }
    /* Exactly 16 bytes of complete, position-independent prologue instructions. */
    unsigned char *trampoline=VirtualAlloc(NULL,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
    if(!trampoline)return FALSE;
    memcpy(trampoline,found,16);write_jump(trampoline+16,found+16);
    DWORD old;
    if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return FALSE;}
    FlushInstructionCache(GetCurrentProcess(),trampoline,30);
    original_decoder=(DecoderFunction)trampoline;
    if(!VirtualProtect(found,16,PAGE_EXECUTE_READWRITE,&old)){original_decoder=NULL;VirtualFree(trampoline,0,MEM_RELEASE);return FALSE;}
    write_jump(found,decode_hook);found[14]=found[15]=0x90;
    DWORD ignored;VirtualProtect(found,16,old,&ignored);
    FlushInstructionCache(GetCurrentProcess(),found,16);
    status("decoder_hook_installed; changes require a source ID or captured input fingerprint plus exact original text");
    return TRUE;
}
static void number(wchar_t *destination,WORD value,int digits) {
    for(int i=digits-1;i>=0;i--){destination[i]=(wchar_t)(L'0'+value%10);value/=10;}
}
static void open_logs(void) {
    wchar_t path[MAX_PATH],name[80]=L"hkmw4-logs\\capture-00000000-000000.jsonl";
    make_path(path,L"hkmw4-logs");CreateDirectoryW(path,NULL);
    SYSTEMTIME time;GetLocalTime(&time);
    number(name+19,time.wYear,4);number(name+23,time.wMonth,2);number(name+25,time.wDay,2);
    number(name+28,time.wHour,2);number(name+30,time.wMinute,2);number(name+32,time.wSecond,2);
    make_path(path,name);
    capture_file=CreateFileW(path,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
    make_path(path,L"hkmw4-logs\\status.log");
    status_file=CreateFileW(path,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
}

/* Isolated data-path checks can use these exports without hooking another app. */
__declspec(dllexport) BOOL WINAPI HKInitializeTest(const wchar_t *folder) {
    size_t n=wide_length(folder);if(n>=MAX_PATH)return FALSE;
    memcpy(directory,folder,(n+1)*sizeof(wchar_t));return load_tables();
}
__declspec(dllexport) const char *WINAPI HKTranslateTest(uint64_t id,const char *text,DWORD length) {
    return translate(id,0,text,length);
}
__declspec(dllexport) const char *WINAPI HKTranslateWithHashTest(uint64_t id,uint64_t fingerprint,const char *text,DWORD length) {
    return translate(id,fingerprint,text,length);
}
__declspec(dllexport) uint64_t WINAPI HKSourceTest(uint64_t fingerprint,DWORD length) {
    return source_id(fingerprint,length);
}

__declspec(dllexport) BOOL WINAPI HKInitializeCandidateTest(const wchar_t *folder) {
    InitializeCriticalSection(&cache_lock);
    for(int i=0;i<64;i++)InitializeCriticalSection(&cache_shards[i]);
    return HKInitializeTest(folder) && load_mission_rules();
}
__declspec(dllexport) const char * WINAPI HKTranslateCandidateTest(uint64_t id,uint64_t fp,const char *text,DWORD length) {
    const char *first=translate(id,fp,text,length); BOOL chinese=FALSE;
    if(first!=text || !valid_utf8((const unsigned char*)text,length,&chinese))return first;
    return general_translate(text,length,chinese);
}
BOOL WINAPI DllMain(HINSTANCE instance,DWORD reason,LPVOID reserved) {
    (void)reserved;
    if(reason==DLL_PROCESS_ATTACH) {
        module_handle=instance;DisableThreadLibraryCalls(instance);
        wchar_t executable[MAX_PATH];
        if(!GetModuleFileNameW(NULL,executable,MAX_PATH))return TRUE;
        wchar_t *filename=executable;
        for(wchar_t *p=executable;*p;p++)if(*p==L'\\'||*p==L'/')filename=p+1;
        static const wchar_t required[]=L"sp26-cod.exe";
        if(wide_length(filename)!=sizeof(required)/sizeof(required[0])-1 || !equal_bytes(filename,required,sizeof(required)))return TRUE;
        if(!GetModuleFileNameW(instance,directory,MAX_PATH))return TRUE;
        for(size_t i=wide_length(directory);i>0;i--)if(directory[i-1]==L'\\'){directory[i-1]=0;break;}
        InitializeCriticalSection(&capture_lock);
        InitializeCriticalSection(&cache_lock);
        for(int i=0;i<64;i++)InitializeCriticalSection(&cache_shards[i]);
        /* Capture and status logging are disabled in the production build. */
        if(!load_tables()||!load_mission_rules()){status("data_validation_failed: no runtime patch applied");return TRUE;}
        if(!install_hook())status("hook_inactive");
    } else if(reason==DLL_PROCESS_DETACH) {
        if(capture_file!=INVALID_HANDLE_VALUE)CloseHandle(capture_file);
        if(status_file!=INVALID_HANDLE_VALUE)CloseHandle(status_file);
    }
    return TRUE;
}
