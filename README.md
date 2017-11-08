
# first-shell
here are code for make a simple shell  
This code is basaded there shell versions on the Bare Bones kernel of Osdev.org, this code must be placed just before the following lines:

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

== Credits: ==

Luis jordany Chajil Patzan        0900-13-18667
Gabriel Medardo Ortiz García      0900-11-8756
Walfred Magdiel  Chipix  Tiniguar 0900-15-18006
Byron Ramos                       0900-13-10118
Levi Esau Aguilar Carrillo        0900-11-8733
