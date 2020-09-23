// Template to complete the patchsym program which can retrive global
// symbol values or change them. Sections that start with a CAPITAL in
// their comments require additions and modifications to complete the
// program.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

int DEBUG = 0;                  // controls whether to print debug messages

#define GET_MODE 1              // only get the value of a symbol
#define SET_MODE 2              // change the value of a symbol

int main(int argc, char **argv){
  // PROVIDED: command line handling of debug option
  if( argc > 1 && strcmp(argv[1], "-d")==0 ){
    DEBUG = 1;                  // check 1st arg for -d debug
    argv++;                     // shift args forward if found
    argc--;
  }

  if(argc < 4){
    printf("usage: %s [-d] <file> <symbol> <type> [newval]\n",argv[0]);
    return 0;
  }

  int mode = GET_MODE;          // default to GET_MODE
  char *new_val = NULL;
  //
  if(argc == 5){                // if an additional arg is provided run in SET_MODE

    printf("SET mode\n");
    mode = SET_MODE;
    new_val = argv[4];
  }
  else{
    printf("GET mode\n");
  }
  char *objfile_name = argv[1];
  char *symbol_name = argv[2];
  char *symbol_kind = argv[3];

  // PROVIDED: open file to get file descriptor
  int fd = open(objfile_name, O_RDWR);

  // DETERMINE size of file and create read/write memory map of the file
  struct stat stat_buf;
  fstat(fd, &stat_buf);                      // get stats on the open file such as size
  int size = stat_buf.st_size;               // size for mmap()'ed memory is size of file
  char *file_bytes =                         // pointer to file contents
    mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,  // call mmap with given size and file descriptor
         fd, 0);                             // read only, potentially share, offset 0
  // CREATE A POINTER to the intial bytes of the file which are an ELF64_Ehdr struct
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *) file_bytes;

  // CHECK e_ident field's bytes 0 to for for the sequence {0x7f,'E','L','F'}.
  int ident_matches =                                   // check the first bytes to ensure correct file format
    ehdr->e_ident[0] == 0x7F &&
    ehdr->e_ident[1] == 'E'  &&
    ehdr->e_ident[2] == 'L'  &&
    ehdr->e_ident[3] == 'F';
  // Exit the program with code 1 if the bytes do not match                                                             // ????????????????????????/printf("%s\n", );
  if(!ident_matches){
    printf("ERROR: Magic bytes wrong, this is not an ELF file\n");
    munmap(file_bytes, size);                  // unmap and close file
    close(fd);
    return 1;
  }


  // PROVIDED: check for a 64-bit file
  if(ehdr->e_ident[EI_CLASS] != ELFCLASS64){
    printf("ERROR: Not a 64-bit file ELF file\n");
    // UNMAP, CLOSE FD
    munmap(file_bytes, size);                  // unmap and close file
    close(fd);
    return 1;
  }

  // PROVIDED: check for x86-64 architecture
  if(ehdr->e_machine != EM_X86_64){
    printf("ERROR: Not an x86-64 file\n");
    // UNMAP, CLOSE FD
    munmap(file_bytes, size);                  // unmap and close file
    close(fd);
    return 1;
  }

  // DETERMINE THE OFFSET of the Section Header Array (e_shoff), the
  // number of sections (e_shnum), and the index of the Section Header
  // String table (e_shstrndx). These fields are from the ELF File
  // Header.
  int offset = ehdr->e_shoff;       //5000
  int shnum = ehdr->e_shnum;        //6 sections
  int shstrndx = ehdr->e_shstrndx;  //2

  // SET UP a pointer to the array of section headers. Use the section
  // header string table index to find its byte position in the file
  // and set up a pointer to it.
  Elf64_Shdr *sec_hdrs = (Elf64_Shdr *) (file_bytes + offset);       //5000
  char *sec_offset = file_bytes + sec_hdrs[shstrndx].sh_offset;      //1000
  // potentially introduce new variables.

  unsigned int index_sym = -1;
  unsigned int index_str = -1;
  unsigned int index_data = -1;      //section index


  unsigned long sym_offset = 0;
  unsigned long str_offset = 0;
  unsigned long data_offset = 0;       //bytes offset from start of file


  // SEARCH the Section Header Array for sections with names .symtab
  // (symbol table) .strtab (string table), and .data (initialized
  // data sections).  Note their positions in the file (sh_offset
  // field).  Also note the size in bytes (sh_size) and and the size
  // of each entry (sh_entsize) for .symtab so its number of entries
  // can be computed. Finally, note the .data section's index (i value
  // in loop) and its load address (sh_addr).

  for(int i=0; i<shnum; i++){
    if (strcmp(sec_offset + sec_hdrs[i].sh_name,".symtab") == 0){
      index_sym = i;
      sym_offset = sec_hdrs[i].sh_offset;

   }
    if (strcmp(sec_offset + sec_hdrs[i].sh_name,".strtab") == 0){
      index_str = i;
      str_offset = sec_hdrs[i].sh_offset;

  }
    if (strcmp(sec_offset + sec_hdrs[i].sh_name,".data") == 0){
      index_data = i;
      data_offset = sec_hdrs[i].sh_offset;
    }
  }
  // ERROR check to ensure everything was found based on things that
  // could not be found.
  if(index_sym ==-1){             //if index was still -1, which means didn't find section
    printf("ERROR: Couldn't find symbol table\n");
    munmap(file_bytes, size);                  // unmap and close file
    close(fd);
    return 1;
  }

  if(index_str ==-1){
    printf("ERROR: Couldn't find string table\n");
    munmap(file_bytes, size);                  // unmap and close file
    close(fd);
    return 1;
  }

  if(index_data ==-1){
    printf("ERROR: Couldn't find data section\n");
    munmap(file_bytes, size);                  // unmap and close file
    close(fd);
    return 1;
  }

  // PRINT info on the .data section where symbol values are stored

  printf(".data section\n");
  printf("- %hd section index\n",index_data);
  printf("- %lu bytes offset from start of file\n",data_offset);                            //3500
  printf("- 0x%lx preferred virtual address for .data\n",sec_hdrs[index_data].sh_addr);     //0x4020


  // PRINT byte information about where the symbol table was found and
  // its sizes. The number of entries in the symbol table can be
  // determined by dividing its total size in bytes by the size of
  // each entry.

  printf(".symtab section\n");
  printf("- %hd section index\n",index_sym);
  printf("- %lu bytes offset from start of file\n",sym_offset);      //600
  printf("- %lu bytes total size\n",sec_hdrs[index_sym].sh_size);
  printf("- %lu bytes per entry\n",sec_hdrs[index_sym].sh_entsize);
  printf("- %lu entries\n",sec_hdrs[index_sym].sh_size/ sec_hdrs[index_sym].sh_entsize);
  //

  // SET UP pointers to the Symbol Table and associated String Table
  // using offsets found earlier. Then SEARCH the symbol table for for
  // the specified symbol.


  Elf64_Sym *symtab = (Elf64_Sym *) (file_bytes + sym_offset);             // 0 + 600
  char* symtab_offset = file_bytes + str_offset;                           // 0 + 500

  for(int i=0; i < sec_hdrs[index_sym].sh_size/ sec_hdrs[index_sym].sh_entsize; i++){


    int has_symbol = 0;
    if(strcmp(symtab_offset + symtab[i].st_name, symbol_name) == 0){ //500 + 28 = 528 = bytes pos of string1
      // If symbol at position i matches the 'symbol_name' variable
      // defined at the start of main(), it is the symbol to work on.
      // PRINT data about the found symbol.
      has_symbol = 1;

    }
    if(has_symbol){
      printf("Found Symbol '%s'\n",symbol_name);
      printf("- %d symbol index\n",i);
      printf("- 0x%lx value\n",symtab[i].st_value);
      printf("- %lu size\n",symtab[i].st_size);
      printf("- %hu section index\n",symtab[i].st_shndx);

      // CHECK that the symbol table field st_shndx matches the index
      // of the .data section; otherwise the symbol is not a global
      // variable and we should bail out now.
      if( symtab[i].st_shndx != index_data ){
        printf("ERROR: '%s' in section %hd, not in .data section %hd\n",symbol_name, symtab[i].st_shndx, index_data);
        munmap(file_bytes, size);                  // unmap and close file
        close(fd);
        return 1;
      }

      // CALCULATE the offset of the value into the .data section. The
      // 'value' field of the symbol is its preferred virtual
      // address. The .data section also has a preferred load virtual
      // address. The difference between these two is the offset into
      // the .data section of the mmap()'d file.
      unsigned long val_offset = symtab[i].st_value-sec_hdrs[index_data].sh_addr;    // example: 0x4040 - 0x4020 = 32
      char *val_addr_in_data = (char *)(file_bytes + data_offset) + val_offset;      // 0 + 3500 + 32
      printf("- %ld offset in .data of value for symbol\n",val_offset);             //the offset of the value into the .data section.

      // Symbol found, location in .data found, handle each kind (type
      // in C) of symbol value separately as there are different
      // conventions to change a string, an int, and so on.

      // string is the only required kind to handle
      if( strcmp(symbol_kind,"string")==0 ){
        // PRINT the current string value of the symbol in the .data section

        printf("string value: '%s'\n",val_addr_in_data);

        // Check if in SET_MODE in which case change the current value to a new one
        if(mode == SET_MODE){

          // CHECK that the length of the new value of the string in
          // variable 'new_val' is short enough to fit in the size of
          // the symbol.

          if( strlen(new_val) >  symtab[i].st_size ){
            // New string value is too long, print an error
            printf("ERROR: Cannot change symbol '%s': existing size too small\n",symbol_name);
            printf("Cur Size: %lu '%s'\n", symtab[i].st_size, val_addr_in_data);
            printf("New Size: %lu '%s'\n", strlen(new_val) + 1  , new_val);       //+1 for test 10
            // UNMAP, CLOSE FD
            munmap(file_bytes, size);                  // unmap and close file
            close(fd);
            return 1;
          }
          else{
            // COPY new string into symbols space in .data as it is big enough
            sprintf(val_addr_in_data, "%s", new_val);                     //sprintf( location(offset) , type, value)

            printf("New val is: '%s'\n", val_addr_in_data);
          }
        }
      }

      // OPTIONAL: fill in else/ifs here for other types on might want
      // to support such as int and double

      else{
        printf("ERROR: Unsupported data kind '%s'\n",symbol_kind);
        // UNMAP, CLOSE FD
        munmap(file_bytes, size);                  // unmap and close file
        close(fd);
        return 1;
      }

      // succesful completion of getting or setting the symbol
      // UNMAP, CLOSE FD
      munmap(file_bytes, size);                  // unmap and close file
      close(fd);
      return 0;
    }
  }

  // Iterated through whole symbol tabel and did not find symbol, error out.
  printf("ERROR: Symbol '%s' not found\n",symbol_name);
  // UNMAP, CLOSE FD
  munmap(file_bytes, size);                  // unmap and close file
  close(fd);
  return 1;
}
