#include "Assembler.h"

//======================================================

int main(int argc, const char* argv[])
{
    FILE* input = fopen(argv[1], "rb");

    if (input == NULL)
    {
        printf("File is not found, ERROR\n");
        fflush(stdout);
        return 1;
    }

    struct Text input_text = {};
    Create_text(input, &input_text);

    fclose(input);
    FILE* listing = fopen("ASM_listing.txt", "wb");
    FILE* code    = fopen("code.txt",        "wb");

    Compiletion(&input_text, listing, code);

    fclose(listing);
    fclose(code);

    Free_memory(&input_text);
}