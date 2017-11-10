/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
   #if !defined(__cplusplus)
   #include <stdbool.h> /* C doesn't have booleans by default. */
   #endif
   #include <stddef.h>
   #include <stdint.h>

   /* Check if the compiler thinks we are targeting the wrong operating system. */
   #if defined(__linux__)
   #error "Usted no esta usando un cross-compiler, you will most certainly run into trouble"
   #endif
    
   /* This tutorial will only work for the 32-bit ix86 targets. */
   #if !defined(__i386__)
   #error "Este tutorial necesita ser compilado con un compilador ix86-elf"
   #endif

   /*================================*/
    //cadena de operaciones 
   const char* cadenaA;
   int primer_termino;
   int segundo_termino;
   char operando;
   /*================================*/

   /* Hardware text mode color constants. */
   enum vga_color {
       VGA_COLOR_BLACK = 0,
       VGA_COLOR_BLUE = 1,
       VGA_COLOR_GREEN = 2,
       VGA_COLOR_CYAN = 3,
       VGA_COLOR_RED = 4,
       VGA_COLOR_MAGENTA = 5,
       VGA_COLOR_BROWN = 6,
       VGA_COLOR_LIGHT_GREY = 7,
       VGA_COLOR_DARK_GREY = 8,
       VGA_COLOR_LIGHT_BLUE = 9,
       VGA_COLOR_LIGHT_GREEN = 10,
       VGA_COLOR_LIGHT_CYAN = 11,
       VGA_COLOR_LIGHT_RED = 12,
       VGA_COLOR_LIGHT_MAGENTA = 13,
       VGA_COLOR_LIGHT_BROWN = 14,
       VGA_COLOR_WHITE = 15,
   };
    
   static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
       return fg | bg << 4;
   }
    
   static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
       return (uint16_t) uc | (uint16_t) color << 8;
   }
    
   //cuenta la cantidad de simbolos en la cadena
   size_t strlen(const char* str) {
       size_t len = 0;
       while (str[len])
           len++;
       return len;
   }
    
   static const size_t VGA_WIDTH = 70;
   static const size_t VGA_HEIGHT = 25;
    
   size_t terminal_row;
   size_t terminal_column;
   uint8_t terminal_color;
   uint16_t* terminal_buffer;
    
   void terminal_initialize(void) {
       terminal_row = 0;
       terminal_column = 0;
       terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
       terminal_buffer = (uint16_t*) 0xB8000;
       for (size_t y = 0; y < VGA_HEIGHT; y++) {
           for (size_t x = 0; x < VGA_WIDTH; x++) {
               const size_t index = y * VGA_WIDTH + x;
               terminal_buffer[index] = vga_entry(' ', terminal_color);
           }
       }
   }
    
   void terminal_setcolor(uint8_t color) {
       terminal_color = color;
   }
    
   void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
       const size_t index = y * VGA_WIDTH + x;
       terminal_buffer[index] = vga_entry(c, color);        //esta almacenando todo lo que escribimos
   }
    
   void terminal_putchar(char c) {  //recibe los caracteres ya separados para imprimir  
       terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
       if (++terminal_column == VGA_WIDTH) {
           terminal_column = 0;
           if (++terminal_row == VGA_HEIGHT)
               terminal_row = 0;
       }
   }
    
   void terminal_write(const char* data, size_t size) {  //separa caracter por caracter
       for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
   }
    
   void terminal_writestring(const char* data) {  //lee la cadena y la longitud de la misma
       terminal_write(data, strlen(data));
   }
  /*========================================================================================================================================*/ 
   /*edicion para calculadora
   *En este punto se lee todo el codigo que genera el 
   *SHELL  de la calculadora*/

    //borrar un caracter
    void borrar(){
        int x = terminal_row;
        terminal_row = x-1;
        char* aux = cadenaA[1];
        for(int i = 1; i< strlen(cadenaA)-1; i++){
            char* aux2 = aux;
            aux = cadenaA[i] + aux2;
        }
    }
   
    //Salto de linea
    void salto_linea(int row){
        terminal_row = terminal_row + row;
        terminal_column = 0;
    }

    //80 caracteres de largo en la pantalla
    void ponte_en(int x, int y){        //similar a la funcion goto(x,y) de c++
        terminal_column = x;
        terminal_row = y;
    }

   /*uint8_t, uint8_t, uint8_t cuenta la asignacion de memoria en bytes*/
    //mensajes de error sintactico borra la linea de entrada
    mensaje_error(char* mensj){
        int x = terminal_row;
        int y = terminal_column;
        ponte_en(10,5);
        terminal_writestring(mensj);
        for(int i=0; i<VGA_WIDTH; i++){
            ponte_en(x,i);
            terminal_writestring(" ");
        }
        cadenaA=0;
    }

    //pasa a conversor
    int posicion_decimal(int x, int i){     //coloca el valor en el orden decimal correcto
        for(int j = 1; j<i; j++){
            x= x*10;
        }
    }
    void conversor(){   //convierte la cadena en un entero y lo pasa al termino que queda
    }

   //valida el caracter antes de incluirlo a la pila
   void acumulador(char letra, bool corte){   //corte indica si el dato es correcto
    if(corte == false){
        char* cadenaB;
        cadenaB= cadenaA;
        cadenaA = cadenaB + letra;
    }else{
        operando = letra;
        conversor();
    }
   }

   /*EAX es un resgistro de acumulacion extendido. Almacena el valor de retorno de una 
    *funcion y lo utiliza como contenedor para operaciones matematicas sencillas*/
   //estructura inb y outb
   static inline uint8_t inb(uint16_t port) //inicia funcion que conectara con el driver del teclado
   {
       uint8_t ret; //crea la variable de respuesta
       asm volatile ( "inb %1, %0"   //asignacion binaria de encendido y apagado del puerto
                      : "=a"(ret)   //asigna la respuesta a la variable ret
                      : "Nd"(port) ); //indicacion de las restricciones N(8-bit enteros no definidos, para instruciones de salida y entrada) y d(registros) para el dato obtenido por port
       return ret;  //devulve el estado generado por la instruccion =a
   }
   /*la combinacion Nd genera la posibilidad de administrar espacios para ahorrar recursos*/
   
   static inline void outb(uint16_t port, uint8_t val)
   {
       asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
   }

   //otras funciones
   static inline uint32_t farpeekl(uint16_t sel, void* off)
   {
       uint32_t ret;
       asm ( "push %%fs\n\t"
             "mov  %1, %%fs\n\t"
             "mov  %%fs:(%2), %0\n\t"
             "pop  %%fs"
             : "=r"(ret) : "g"(sel), "r"(off) );
       return ret;
   }

   static inline void farpokeb(uint16_t sel, void* off, uint8_t v)
   {
       asm ( "push %%fs\n\t"
             "mov  %0, %%fs\n\t"
             "movb %2, %%fs:(%1)\n\t"
             "pop %%fs"
             : : "g"(sel), "r"(off), "r"(v) );
       /* TODO: Should "memory" be in the clobber list here? */
   }

   static inline bool are_interrupts_enabled()
   {
       unsigned long flags;
       asm volatile ( "pushf\n\t"
                      "pop %0"
                      : "=g"(flags) );
       return flags & (1 << 9);
   }
   
   //asignacion de teclado 
   /*ATENCION
    * cuando detecta un numero lo acumula en un vector de caracteres, cuando ve un simbolo diferente, 
    * pasa los digitos guardados a un valor entero y vacia el vector, el valor entero se guarda en una
    * variable a y el siguiente valor en variable b*/

   void getScancode() {
        //variable que asigna el los valores obtenidos por inb()
        char c=inb(0x60); //0x64
        char numchar;
        bool enter = false;
        do {       //no te detengas!! realiza esta actividad...
            if(inb(0x60)!=c) {  //si no encuentra un valor 0
                c=inb(0x60);    //reinicio a cero
                if(c>0){        //si detecto un valor valido
                    switch(c){  //segun el valor encontrado
                        case 0x02 : numchar = '1'; 
                        acumulador(numchar, false);
                        terminal_putchar(numchar);                    
                        break;
                        case 0x03 : numchar = '2'; 
                        acumulador(numchar, false);
                        terminal_putchar(numchar);
                        break;
                        case 0x04 : numchar = '3'; 
                        acumulador(numchar, false);
                        terminal_putchar(numchar);
                        break;
                        case 0x05 : numchar = '4';
                        acumulador(numchar, false); 
                        terminal_putchar(numchar);
                        break;
                        case 0x06 : numchar = '5'; 
                        acumulador(numchar, false);
                        terminal_putchar(numchar);
                        break;
                        case 0x07 : numchar = '6'; 
                        acumulador(numchar, false);
                        terminal_putchar(numchar);
                        break;
                        case 0x08 : numchar = '7'; 
                        acumulador(numchar, false);
                        terminal_putchar(numchar);
                        break;
                        case 0x09 : numchar = '8'; 
                        acumulador(numchar, false);
                        terminal_putchar(numchar);
                        break;
                        case 0x0A : numchar = '9';
                        acumulador(numchar, false); 
                        terminal_putchar(numchar);
                        break;
                        case 0x0B : numchar = '0'; 
                        acumulador(numchar, false);
                        terminal_putchar(numchar);
                        break;
                        case 0x1F : numchar = 's';      //suma
                        acumulador('s',true);
                        terminal_putchar(numchar);
                        break;
                        case 0x13 : numchar = 'r';      //resta
                        acumulador('r',true);
                        terminal_putchar(numchar);
                        break;
                        case 0x32 : numchar = 'm';      //multiplica
                        acumulador('m',true);
                        terminal_putchar(numchar);
                        break;
                        case 0x20 : numchar = 'd';      //divide
                        acumulador('d',true);
                        terminal_putchar(numchar);
                        break;
                        /*case 0x39  : numchar = '_'; //ESPACE guarda los valores a en otra variable y se alista para valores b
                        acumulador('_',true);
                        terminal_putchar(numchar);
                        break;*/
                        case 0x1C : enter = true;
                        numchar = 0;
                        break;
                    }
                }
            }
        } while(enter != true);  //has esta actividad al menos UNA VEZ hasta que se presione ENTER
    }  
    
    //calculadora
    void suma(){
        int resultado = primer_termino + segundo_termino;
        terminal_writestring("el resultado de su suma es de: " + resultado);
    }

    void resta(){
        int resultado = primer_termino - segundo_termino;
        terminal_writestring("el resultado de su resta es de: " + resultado);
    }

    calculadora(){
        terminal_initialize();
        terminal_writestring("ingrese primer termino");
        getScancode();
        terminal_writestring("ingrese segundo termino")
        getScancode();
    }
    

       //menu
   void menu(){
    bool y = false;
    do{             //repite hasta que sea un valor valido
        cadenaA = 0;
        getScancode();
        switch(cadenaA[0]){
            case 1 : calculadora(); y=true;
            case 2 : salir(); y=true;
            default: ponte_en(1,7); terminal_writestring("caracter no valido");
    }while(y!=true);
    }
   
  /*=========================================================================================================================================*/ 
    
   #if defined(__cplusplus)
   extern "C" /* Use C linkage for kernel_main. */
   #endif
   void kernel_main(void) {
       /* Initialize terminal interface */
       terminal_initialize();   //se puede usar para limpiar la pantalla
    
       /* Newline support is left as an exercise. */
       terminal_writestring("Hola , Somos el grupo Open suse! Luis Chajil ");
       ponte_en(0,3);
       terminal_writestring("MENU 1.Operar 2.Salir");
       ponte_en(3,5);
       menu();
   }
   
