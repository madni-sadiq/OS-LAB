#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; // Array for storing opened files

/*
int simplefs_allocInode();
void simplefs_freeInode(int inodenum);
void simplefs_readInode(int inodenum, struct inode_t *inodeptr);
void simplefs_writeInode(int inodenum, struct inode_t *inodeptr); 
int simplefs_allocDataBlock();
void simplefs_freeDataBlock(int blocknum);
void simplefs_readDataBlock(int blocknum, char *buf);
void simplefs_writeDataBlock(int blocknum, char *buf);
void simplefs_dump();
*/

int get_inode(char *filename)
{
    struct superblock_t *superblock = (struct superblock_t *)malloc(sizeof(struct superblock_t));

    int inode_num;
    struct inode_t *inodeptr = (struct inode_t *)malloc(sizeof(struct inode_t));

    simplefs_readSuperBlock(superblock);

    for (int i = 0; i < NUM_INODES; i++)
    {
        if (superblock->inode_freelist[i] == INODE_IN_USE)
        {
            simplefs_readInode(inode_num, inodeptr);
            if (!strcmp(inodeptr->name, filename))
            {
                free(inodeptr);
                free(superblock);
                return i;
            }
        }
    }
    free(inodeptr);
    free(superblock);
    return -1;
}

int simplefs_create(char *filename)
{
    /*
        Create file with name `filename` from disk
    */
    int inode_num;
    struct inode_t *inodeptr = (struct inode_t *)malloc(sizeof(struct inode_t));

    if (get_inode(filename) == -1)
        return -1;

    inode_num = simplefs_allocInode();
    if (inode_num == -1)
        return -1;

    simplefs_readInode(inode_num, inodeptr);
    strcpy(inodeptr->name, filename);
    inodeptr->status = INODE_IN_USE;
    simplefs_writeInode(inode_num, inodeptr);
    free(inodeptr);
}

void simplefs_delete(char *filename)
{
    /*
        delete file with name `filename` from disk
    */
    int inode_num;

    if ((inode_num = get_inode(filename)) == -1)
        return;
    void simplefs_freeInode(inode_num);
    void simplefs_freeDataBlock(blocknum);
}

int simplefs_open(char *filename)
{
    /*
        open file with name `filename`
    */
    int inode_num = get_inode(filename);
    for(int i = 0; i < MAX_OPEN_FILES; i++){
        if(file_handle_array[i].inode_number == -1){
            file_handle_array[i].inode_number = inode_num;
            return i;
        }
    }
    return -1;
}

void simplefs_close(int file_handle)
{
    /*
        close file pointed by `file_handle`
    */
    file_handle_array[file_handle].inode_number = -1;
    file_handle_array[file_handle].offset = 0;
}

int simplefs_read(int file_handle, char *buf, int nbytes)
{
    /*
        read `nbytes` of data into `buf` from file pointed by `file_handle` starting at current offset
    */
    return -1;
}

int simplefs_write(int file_handle, char *buf, int nbytes)
{
    /*
        write `nbytes` of data from `buf` to file pointed by `file_handle` starting at current offset
    */
    int dBlock_no;
    dBlock_no = simplefs_allocDataBlock();
    return -1;
}

int simplefs_seek(int file_handle, int nseek)
{
    /*
       increase `file_handle` offset by `nseek`
    */
    return -1;
}