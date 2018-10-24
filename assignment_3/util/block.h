#ifndef H_BLOCK
# define H_BLOCK
# define SIZE_BLOCK(x) (x.idx2 - x.idx1)

/* Struct for representing a block */
struct block_t{
    int idx1;
    int idx2;
};

/* Devides blocks into n mutual exclusive blocks which is ordered in size and
   their union is equal to the block. */
struct block_t * devide_block(const struct block_t block, const int count);
void print_block(struct block_t block);

struct block_t *
devide_block(const struct block_t block, const int n)
{
    struct block_t * blocks = \
        (struct block_t *)malloc(sizeof(struct block_t) * n);
    int block_size = SIZE_BLOCK(block);
    int new_block_size = (int)(block_size / n);
    int mod = block_size % n;

    int start_idx = block.idx1;
    if (mod){
        new_block_size++;
        for (int i = 0 ; i < mod ; i++){
            blocks[i].idx1 = start_idx;
            blocks[i].idx2 = start_idx + new_block_size;
            start_idx += new_block_size;
        }
        new_block_size--;
    }

    for (int i = mod; i < n; i++){
        blocks[i].idx1 = start_idx;
        blocks[i].idx2 = start_idx + new_block_size;
        start_idx += new_block_size;
    }
    return blocks;
}

/* Prints start, fixed and size of a block. */
void print_block(struct block_t block)
{
    printf(
        "Block: idx1: %d, idx2: %d, size: %d \n",
        block.idx1,
        block.idx2,
        SIZE_BLOCK(block));
}

#endif
