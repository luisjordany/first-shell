# first-shell
here are code for make a simple shell  
Usualy in this area there are shell versions based on the Bare Bones kernel of Osdev.org, this code must be placed just before the following lines

#if defined(__cplusplus)
   extern "C" /* Use C linkage for kernel_main. */
   #endif
   void kernel_main(void) {
       /* Initialize terminal interface */
       terminal_initialize();
    
       /* Newline support is left as an exercise. */
       terminal_writestring("Hola mundo!!");
       terminal_writestring("menu \n 1.Operar 2.Salir");
       //<llama a la siguiente funcion>
   }
