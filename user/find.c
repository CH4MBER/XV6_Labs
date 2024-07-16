#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
    if(re[0] == '^')
        return matchhere(re+1, text);
    do{  // must look at empty string
        if(matchhere(re, text))
        return 1;
    }while(*text++ != '\0');
    return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
    if(re[0] == '\0')
        return 1;
    if(re[1] == '*')
        return matchstar(re[0], re+2, text);
    if(re[0] == '$' && re[1] == '\0')
        return *text == '\0';
    if(*text!='\0' && (re[0]=='.' || re[0]==*text))
        return matchhere(re+1, text+1);
    return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
    do{  // a * matches zero or more instances
        if(matchhere(re, text))
        return 1;
    }while(*text!='\0' && (*text++==c || c=='.'));
    return 0;
}

char* fmtname(char *path);
void find(char *path, char *re);

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

  // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

  // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memset(buf, 0, sizeof(buf));
    memmove(buf, p, strlen(p));
    // memmove(buf, p, strlen(p));
    // memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void
find(char *path,char *re)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    case T_FILE:
        if (match(re,fmtname(path)))
        {
            printf("%s\n", path);
        }
        
        // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
        break;

    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("fid: cannot stat %s\n", buf);
                continue;
            }

            if(strcmp(".", fmtname(buf)) == 0 || strcmp("..", fmtname(buf)) == 0){
            //printf("%s %d %d %d\n", buf, st.type, st.ino, st.size);
                continue;
            }
            else{
                find(buf,re);
            }
            
        // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
        
            }

        break;
        }
    close(fd);
    return;
}

int main(int argc, char *argv[])
{


    if(argc != 3){
        printf("wrong argument number");
        exit(1);
    }
    else{
        find(argv[1],argv[2]);
        exit(0);
    }

    exit(0);
    return 0;
}