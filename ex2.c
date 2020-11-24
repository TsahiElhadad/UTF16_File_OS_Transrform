#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Struct for configure the flags we get.
 */
struct Flag{
    char * src;
    char * dst;
    bool isWinSrc;// check if the src flag is -win.
    bool isWinDst;// check if the dst flag is -win.
};

typedef enum printDbySys{
    pUnix = 0x000a,// '\n'
    pMac = 0x000d, // '\r'
    pWin = 0x000d000a // '\r\n'
}printDbySys;

/**
 * Function that copy src file to dst file(making dst file) without flags.
 * @param srcFileName - get source file.
 * @param dstFileName - get name for new file to copy to.
 */
void copyNoFlags(char* srcFileName, char * dstFileName){
    FILE* fileSrcP;// pointer to src file.
    fileSrcP = fopen(srcFileName, "rb");// open in mode binary.
    if (fileSrcP) {// if open file success.
        FILE *filePtoDst = fopen(dstFileName, "wb");// open new dst file.
        char buffer[2];// use buffer of 2 bytes each letter for UTF 16
        size_t n = 1;
        // read from the file untill error or end of file.
        // on success reads n items and return n, on fail return less than n.
        while (1 <= (n = fread(buffer, sizeof(buffer), n, fileSrcP))) {
            fwrite(buffer, sizeof(buffer), n, filePtoDst);// write to the file.
        }
    }
}

/**
 * Function that get 2 flags and bulid a strct Flag that notice whice combination
 * of flags are: win and unix, win and mac, mac and unix...
 * @param flag1 - flag of -win/-unix/-mac.
 * @param flag2 - flag of -win/-unix/-mac.
 * @return - struct Flag of the src flag and the dst flag we get.
 */
struct Flag checkFlags(char* flag1, char* flag2){
    struct Flag flagCheck;
    if (strcmp(flag1,"-unix") == 0 && strcmp(flag2,"-win") == 0){
        flagCheck.src = "\n";
        flagCheck.dst = "\r\n";
        flagCheck.isWinSrc = false;
        flagCheck.isWinDst = true;
        return flagCheck;
    }else if(strcmp(flag1,"-unix") == 0 && strcmp(flag2,"-mac") == 0) {
        flagCheck.src = "\n";
        flagCheck.dst = "\r";
        flagCheck.isWinSrc = false;
        flagCheck.isWinDst = false;
        return flagCheck;
    }else if(strcmp(flag1,"-win") == 0 && strcmp(flag2,"-unix") == 0) {
        flagCheck.src = "\r\n";
        flagCheck.dst = "\n";
        flagCheck.isWinSrc = true;
        flagCheck.isWinDst = false;
        return flagCheck;
    }else if(strcmp(flag1,"-win") == 0 && strcmp(flag2,"-mac") == 0) {
        flagCheck.src = "\r\n";
        flagCheck.dst = "\r";
        flagCheck.isWinSrc = true;
        flagCheck.isWinDst = false;
        return flagCheck;
    }else if(strcmp(flag1,"-mac") == 0 && strcmp(flag2,"-win") == 0) {
        flagCheck.src = "\r";
        flagCheck.dst = "\r\n";
        flagCheck.isWinSrc = false;
        flagCheck.isWinDst = true;
        return flagCheck;
    }else if(strcmp(flag1,"-mac") == 0 && strcmp(flag2,"-unix") == 0) {
        flagCheck.src = "\r";
        flagCheck.dst = "\n";
        flagCheck.isWinSrc = false;
        flagCheck.isWinDst = false;
        return flagCheck;
    }
}

// Function that swap 2 bytes.
void swap(char* b1,char* b2){
    char temp = *b1;
    *b1 = *b2;
    *b2 = temp;
}

/**
 * Function that get source file to copy to new destination file and get
 * two flags: src - which system the file was write, and dst- to which system
 * the new dst file is.
 * @param srcFileName - source file name to copy from.
 * @param dstFileName - destination file name to create.
 * @param flag1 - system of the source file.
 * @param flag2 - system of the dst file.
 */
void copyWith2Flags(char* srcFileName, char * dstFileName, char * flag1,char * flag2, int isKeep) {
    FILE *srcFileP;// pointer to src file.
    srcFileP = fopen(srcFileName, "rb");// open src file
    struct Flag flagStruct = checkFlags(flag1, flag2);// get info of the 2 flags.

    if (srcFileP) {// if open file success.
        FILE *dstFileP = fopen(dstFileName, "wb");// open new dst file.
        char buffer[2];// use buffer of 16 bits for capture letter(2 bytes).
        char temp[2];// temporary buffer for case of win system src or dst.
        size_t n = 1;// read 1 each time.

        if (flagStruct.isWinSrc == true) {// CASE - if src system is win.
            while (1 <= (n = fread(buffer, sizeof(buffer), n, srcFileP))) {// read from srcFile.
                if (strcmp(buffer, "\r") == 0) {// if we catch "\r", check for \n after.
                    strcpy(temp, buffer);// save the "\r" in case we don't have "\n"

                    fread(buffer, sizeof(buffer), 1, srcFileP);// read another letter.
                    if (strcmp(buffer, "\n") == 0) {// if we catch "\n". write to dst print drop.
                        if(isKeep == 0){// needed to switch 2 bytes.
                            strcpy(buffer, flagStruct.dst);// put in buffer the dstFlag sys.
                            swap(&buffer[0],&buffer[1]);// swap between 2 bytes.
                        }else {// isKeep = 1 so keep.
                            strcpy(buffer, flagStruct.dst);
                            fwrite(buffer, sizeof(buffer), 1, dstFileP);
                        }
                    } else {// we don't find \n after \r.
                        if(isKeep == 0){// needed to switch 2 bytes.
                            swap(&temp[0],&temp[1]);
                            swap(&buffer[0],&buffer[1]);
                        }
                        fwrite(temp, sizeof(buffer), 1, dstFileP);// write \r
                        fwrite(buffer, sizeof(buffer), 1, dstFileP);// write buffer(not "\n")
                    }
                } else {// we don't catch "\r", just write the buffer.
                    if(isKeep == 0){// needed to switch 2 bytes.
                        swap(&buffer[0],&buffer[1]);
                    }
                    fwrite(buffer, sizeof(char), 2, dstFileP);
                }
            }
        } else if (flagStruct.isWinDst == true) {// CASE - if dst system is win.
            printf("%d", 1);
            while (1 <= (n = fread(buffer, sizeof(buffer), n, srcFileP))) {// read from srcFile.
                if (strcmp(buffer, flagStruct.src) == 0) {// if catch src print drop.
                    strcpy(buffer, "\r");// put to buffer "\r"
                    if(isKeep == 0){// needed to switch 2 bytes.
                        swap(&buffer[0],&buffer[1]);
                    }
                    fwrite(buffer, sizeof(buffer), 1, dstFileP);// write "\r"
                    strcpy(buffer, "\n");// put to buffer "\n"
                    if(isKeep == 0){// needed to switch 2 bytes.
                        swap(&buffer[0],&buffer[1]);
                    }
                    fwrite(buffer, sizeof(buffer), 1, dstFileP);// write "\n"
                } else {// regular case, just write the buffer bytes to dst file.
                    if(isKeep == 0){// needed to switch 2 bytes.
                        swap(&buffer[0],&buffer[1]);
                    }
                    fwrite(buffer, sizeof(buffer), 1, dstFileP);
                }
            }
        } else {// CASE - no system win in src and dst.
            while (1 <= (n = fread(buffer, sizeof(buffer), n, srcFileP))) {
                if (strcmp(buffer, flagStruct.src) == 0) {// if catch print drop.
                    strcpy(buffer, flagStruct.dst);// copy the dst print drop system.
                    if(isKeep == 0){// needed to switch 2 bytes.
                        swap(&buffer[0],&buffer[1]);
                    }
                    fwrite(buffer, sizeof(buffer), 1, dstFileP);// write buffer to dst file.
                } else {// just write the buffer.
                    if(isKeep == 0){// needed to switch 2 bytes.
                        swap(&buffer[0],&buffer[1]);
                    }
                    fwrite(buffer, sizeof(buffer), 1, dstFileP);
                }
            }
        }
    }
}

/**
 * Main function that get arguments from client.
 * @param argc - number of arguments.
 * @param argv - get char* array of the arguments we get.
 *              3 options available: [src.txt] [dst.txt]
 *                                   [src.txt] [dst.txt] [flag1] [flag2]
 *                                   [src.txt] [dst.txt] [flag1] [flag2] [(-keep / -swap)]
 * @return - no return, just making copy of the source file we get to destination file
 *          according to the 3 options.
 */
int main(int argc, char *argv[]) {
    FILE* srcFile;
    // flag for check if we need switch between 2 bytes of letter. 1= keep, 0 = switch.
    int isKeep = 1;// intial to keep.
    switch (argc) {
        case 3:// Option 1 - src.txt dst.txt
            copyNoFlags(argv[1],argv[2]);
            break;
        case 5:// Option 2 - src.txt dst.txt flag1 flag2
            if (strcmp(argv[3], argv[4]) == 0){// case flags are equals.
                copyNoFlags(argv[1], argv[2]);
            }else{
                // no need to change the bytes of a letter, so ifKeep = 1.
                copyWith2Flags(argv[1], argv[2],
                               argv[3], argv[4], isKeep);
            }
            break;
        case 6:// Option 3 - src.txt dst.txt flag1 flag2 (-keep / -swap)
            if(strcmp(argv[5], "-keep") == 0){// keep the bytes of each letter. ifKeep = 1.
                copyWith2Flags(argv[1], argv[2],
                               argv[3], argv[4], isKeep);
            }else{
                isKeep = 0;// we need to switch between 2 bytes.
                copyWith2Flags(argv[1], argv[2],
                               argv[3], argv[4], isKeep);
            }
            break;
        default:
            break;
    }
    return 0;
}

