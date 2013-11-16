#ifndef KSTRING_H_
#define KSTRING_H_


#define kto_lower(c)          (((c)>='A' && (c)<='Z') ? ((c)|0x20) : (c))
#define kto_upper(c)          (((c)>='a' && (c)<='z') ? ((c)&~0x20) : (c))
#define KTO_STR(s)             #s
#define KPASTE(front, back)    front##back


#endif

