/*
six_xb24zb_automation - a program C made during a project of technologies of control systems in University of Catania with six Digi's XBee wireless modules XB24-Z7WIT-004 revK running in API mode.
This program creates a condition of a realistic management of a home automation system with buttons and lights.
 
Copyright (C) 2014 onwards  Nicola Didomenico (nicola.didomenico@gmail.com)
Copyright (C) 2014 onwards  Riccardo Merenda (riccardomerenda@gmail.com)

Contributors:
Attie Grande (attie@attie.co.uk)
Eng. Giuseppe Avon (giuseppe.avon@hibas.it)
Prof. Robert Faludi  (robert.faludi@nyu.edu)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public Licens along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <xbee.h>
#include <pthread.h>

#define XBEE_IO_IN_DIGITAL 0x03
#define XBEE_IO_OUT_LOW    0x04
#define XBEE_IO_OUT_HIGH   0x05
#define green "\033[01;32m" 

pthread_mutex_t  led_mutex;

struct xbee *xbee;
xbee_err ret;
int led1_status, led2_status, led3_status;
int b1Behaviour=0, b2Behaviour=0;


//ZIGBEE END DEVICE 1  API=1 
struct xbee_con *edcon_b1805b, *cbcon_b1805b;
struct xbee_conAddress ed_addr_b1805b = { .addr64 = { 0x00,0x13,0xA2,0x00,0x40,0xB1,0x80,0x5B }, .addr64_enabled = 1 }; //used

// ZIGBEE ROUTER 1 API=1 
struct xbee_con *rocon_a238d4; 
struct xbee_conAddress ro_addr_a238d4 = { .addr64 = { 0x00,0x13,0xA2,0x00,0x40,0xA2,0x38,0xD4 }, .addr64_enabled = 1 }; //not used

// END DEVICE 2 API=1
struct xbee_con *edcon_af5a99;
struct xbee_conAddress ed_addr_af5a99 = { .addr64 = { 0x00,0x13,0xA2,0x00,0x40,0xAF,0x5A,0x99 }, .addr64_enabled = 1 }; //not used

// END DEVICE 3 API=1
struct xbee_con *edcon_b18074, *cbcon_b18074;
struct xbee_conAddress ed_addr_b18074 = { .addr64 = { 0x00,0x13,0xA2,0x00,0x40,0xB1,0x80,0x74 }, .addr64_enabled = 1 }; //used

// ZIGBEE ROUTER 2 API=1 
struct xbee_con *rocon_8b5f51;
struct xbee_conAddress ro_addr_8b5f51 = { .addr64 = { 0x00,0x13,0xA2,0x00,0x40,0x8B,0x5F,0x51 }, .addr64_enabled = 1 }; //not used

void clear_screen() {
 printf("\x1B[2J\x1B[1;1H");
}

void general_menu() { 
  printf("%s################ GENERAL MENU ################## \n",green);
  printf("0. List Network \n");
  printf("1. Toggle Led 1\n");
  printf("2. Toggle Led 2 \n");
  printf("3. Toggle Led 3 \n");
  printf("4. Button 1 Behaviour\n");
  printf("5. Button 2 Behaviour\n");
  printf("6. Clear all \n");
  printf("7. Exit \n");
  printf("################################################# \n");
}

void list_menu(){
  printf("Led 1, Led 2, Led3, Button Behaviour \n");
  printf("0    , 0    , 0   , 0\n");
  printf("0    , 0    , 1   , 1\n");
  printf("0    , 1    , 0   , 2\n");
  printf("0    , 1    , 1   , 3\n");
  printf("1    , 0    , 0   , 4\n");
  printf("1    , 0    , 1   , 5\n");
  printf("1    , 1    , 0   , 6\n");
  printf("1    , 1    , 1   , 7\n");
}

void list_network_static(){
  if (ed_addr_b1805b.addr64_enabled) {
    printf(" End Device 1, 64-bit address:  0x%02X%02X%02X%02X 0x%02X%02X%02X%02X, attached button ed1 in  DIO1\n",ed_addr_b1805b.addr64[0], ed_addr_b1805b.addr64[1], ed_addr_b1805b.addr64[2],ed_addr_b1805b.addr64[3],ed_addr_b1805b.addr64[4], ed_addr_b1805b.addr64[5], ed_addr_b1805b.addr64[6], ed_addr_b1805b.addr64[7]);
  }
  if (ro_addr_a238d4.addr64_enabled) {
    printf(" Router 1,     64-bit address:  0x%02X%02X%02X%02X 0x%02X%02X%02X%02X, attached led ro1 in  DIO0\n",ro_addr_a238d4.addr64[0], ro_addr_a238d4.addr64[1], ro_addr_a238d4.addr64[2],ro_addr_a238d4.addr64[3],ro_addr_a238d4.addr64[4], ro_addr_a238d4.addr64[5], ro_addr_a238d4.addr64[6], ro_addr_a238d4.addr64[7]);
  }
  if (ro_addr_8b5f51.addr64_enabled) {
    printf(" Router 2,     64-bit address:  0x%02X%02X%02X%02X 0x%02X%02X%02X%02X, attached led ro2 in  DIO0\n",ro_addr_8b5f51.addr64[0], ro_addr_8b5f51.addr64[1], ro_addr_8b5f51.addr64[2],ro_addr_8b5f51.addr64[3],ro_addr_8b5f51.addr64[4], ro_addr_8b5f51.addr64[5], ro_addr_8b5f51.addr64[6], ro_addr_8b5f51.addr64[7]);
  }
  if (ed_addr_af5a99.addr64_enabled) {
    printf(" End Device 2, 64-bit address:  0x%02X%02X%02X%02X 0x%02X%02X%02X%02X, attached led ed2 in  DIO0\n",ed_addr_af5a99.addr64[0], ed_addr_af5a99.addr64[1], ed_addr_af5a99.addr64[2],ed_addr_af5a99.addr64[3],ed_addr_af5a99.addr64[4], ed_addr_af5a99.addr64[5], ed_addr_af5a99.addr64[6], ed_addr_af5a99.addr64[7]);
  }
  if (ed_addr_b18074.addr64_enabled) {
    printf(" End Device 3, 64-bit address:  0x%02X%02X%02X%02X 0x%02X%02X%02X%02X, attached button ed3 in  DIO1\n",ed_addr_b18074.addr64[0], ed_addr_b18074.addr64[1], ed_addr_b18074.addr64[2],ed_addr_b18074.addr64[3],ed_addr_b18074.addr64[4], ed_addr_b18074.addr64[5], ed_addr_b18074.addr64[6], ed_addr_b18074.addr64[7]);
  }
} 

void die(const char *msg, xbee_err ret) {
  fprintf(stderr, "%s returned %d (%s)\n", msg, ret, xbee_errorToStr(ret));
 exit(1);
}

// Call Back End Device 1
void callback_btn_b1805b(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
	if (!data) return;
        struct xbee_con *con_xbee = *data;
        int val;
	if (xbee_pktDigitalGet(*pkt, 1, 0, &val) == XBEE_ENONE) { 
	  if (!val) { 
	       pthread_mutex_lock (&led_mutex);
	       switch (b1Behaviour){
		 case 0: {
		  	  //NO LED AFFECTED
	          break;
		 }
		 case 1: {
                          led3_status=!led3_status;
		  break;
		 }
		 case 2: {
                          led2_status=!led2_status;
                  break; 
		 } 
		 case 3: {
		          led2_status=!led2_status;
                          led3_status=!led3_status;  
		  break;
		 }
		 case 4: {
		          led1_status=!led1_status; 
		  break;
		 }
                 case 5: {
		          led1_status=!led1_status;
                          led3_status=!led3_status;  
		  break;
		 }
                 case 6: {
		          led1_status=!led1_status;
                          led2_status=!led2_status;  
		  break;
		 }
                 case 7: {
		          led1_status=!led1_status;
                          led2_status=!led2_status;
                          led3_status=!led3_status;
		  break;
		 }
               }
               pthread_mutex_unlock (&led_mutex);

               if (led1_status+4 == 4 ) printf("Router 1 - LED OFF \n");  
               else printf("Router 1 - LED ON \n");	
	       xbee_conTx(rocon_a238d4, NULL, "D0%c", led1_status+4);

               if (led2_status+4 == 4 ) printf("End Device 2 - LED OFF \n");  
               else printf("End Device 2 - LED ON \n");
	       xbee_conTx(edcon_af5a99, NULL, "D0%c", led2_status+4);

               if (led3_status+4 == 4 ) printf("Router 2 - LED OFF \n");  
               else printf("Router 2 - LED ON \n");
	       xbee_conTx(rocon_8b5f51, NULL, "D0%c", led3_status+4);
	  }
	}
}

// Call Back End Device 3
void callback_btn_b18074(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
	if (!data) return;
        struct xbee_con *con_xbee = *data;
        int val;
	if (xbee_pktDigitalGet(*pkt, 1, 0, &val) == XBEE_ENONE) {  
          if (!val) { 
               pthread_mutex_lock (&led_mutex);
               switch (b2Behaviour){
		 case 0: {
		  	  //NO LED AFFECTED
	          break;
		 }
		 case 1: {
                          led3_status=!led3_status;
		  break;
		 }
		 case 2: {
                          led2_status=!led2_status;
                  break; 
		 } 
		 case 3: {
		          led2_status=!led2_status;
                          led3_status=!led3_status;  
		  break;
		 }
		 case 4: {
		          led1_status=!led1_status; 
		  break;
		 }
                 case 5: {
		          led1_status=!led1_status;
                          led3_status=!led3_status;  
		  break;
		 }
                 case 6: {
		          led1_status=!led1_status;
                          led2_status=!led2_status;  
		  break;
		 }
                 case 7: {
		          led1_status=!led1_status;
                          led2_status=!led2_status;
                          led3_status=!led3_status;
		  break;
		 }
               }
               pthread_mutex_unlock (&led_mutex);
               if (led1_status+4 == 4 ) printf("Router 1 - LED OFF \n");  
               else printf("Router 1 - LED ON \n");	
	       xbee_conTx(rocon_a238d4, NULL, "D0%c", led1_status+4);

               if (led2_status+4 == 4 ) printf("End Device 2 - LED OFF \n");  
               else printf("End Device 2 - LED ON \n");
	       xbee_conTx(edcon_af5a99, NULL, "D0%c", led2_status+4);

               if (led3_status+4 == 4 ) printf("Router 2 - LED OFF \n");  
               else printf("Router 2 - LED ON \n");
	       xbee_conTx(rocon_8b5f51, NULL, "D0%c", led3_status+4);
	  }
	}
}

int setup(char * device){
  // Set serial xbee
  if ((ret = xbee_setup(&xbee, "xbeeZB", device, 9600))         != XBEE_ENONE) die("xbee_setup()", ret);
   
  // Led 1
  if ((ret = xbee_conNew(xbee, &rocon_a238d4, "Remote AT", &ro_addr_a238d4))           != XBEE_ENONE) die("xbee_conNew(rocon_a238d4)", ret);
  if ((ret = xbee_conTx(rocon_a238d4, NULL, "D0%c", XBEE_IO_OUT_LOW))         != XBEE_ENONE) die("xbee_conTx(rocon_a238d4, D1)", ret); 

  // Led 2  
  if ((ret = xbee_conNew(xbee, &edcon_af5a99, "Remote AT", &ed_addr_af5a99))           != XBEE_ENONE) die("xbee_conNew(edcon_af5a99)", ret);
  if ((ret = xbee_conTx(edcon_af5a99, NULL, "D0%c", XBEE_IO_OUT_LOW ))         != XBEE_ENONE) die("xbee_conTx(edcon_af5a99, D1)", ret); 

  // Led 3  
  if ((ret = xbee_conNew(xbee, &rocon_8b5f51, "Remote AT", &ro_addr_8b5f51))           != XBEE_ENONE) die("xbee_conNew(rocon_8b5f51)", ret);
  if ((ret = xbee_conTx(rocon_8b5f51, NULL, "D0%c", XBEE_IO_OUT_LOW))         != XBEE_ENONE) die("xbee_conTx(rocon_8b5f51, D1)", ret); 

  // Button End Device 1
  if ((ret = xbee_conNew(xbee, &edcon_b1805b, "Remote AT", &ed_addr_b1805b))        != XBEE_ENONE) die("xbee_conNew(edcon_b1805b)", ret);
  if ((ret = xbee_conTx(edcon_b1805b, NULL, "D1%c", XBEE_IO_IN_DIGITAL ))         != XBEE_ENONE) die("xbee_conTx(edcon_b1805b, D1)", ret); 
  // Set IC Button End Device 1
  if ((ret = xbee_conTx(edcon_b1805b, NULL, "IR%c", 0))                       != XBEE_ENONE) die("xbee_conTx(edcon_b1805b, IR)", ret); 
  if ((ret = xbee_conTx(edcon_b1805b, NULL, "IC%c", 0x02))                    != XBEE_ENONE) die("xbee_conTx(edcon_b1805b, IR)", ret); // 0x02 is the mask for D1
  // Call Back edcon_b1805b
  if ((ret = xbee_conNew(xbee, &cbcon_b1805b, "I/O", &ed_addr_b1805b))              != XBEE_ENONE) die("xbee_conNew(cbcon_b1805b)", ret); 
  if ((ret = xbee_conCallbackSet(cbcon_b1805b, callback_btn_b1805b, NULL)) != XBEE_ENONE) die("xbee_conCallbackSet(cbcon_b1805b)", ret);

  // Button End Device 3
  if ((ret = xbee_conNew(xbee, &edcon_b18074, "Remote AT", &ed_addr_b18074))        != XBEE_ENONE) die("xbee_conNew(edcon_b18074)", ret);
  if ((ret = xbee_conTx(edcon_b18074, NULL, "D1%c", XBEE_IO_IN_DIGITAL ))         != XBEE_ENONE) die("xbee_conTx(edcon_b18074, D1)", ret); 
  // Set IC Button End Device 3
  if ((ret = xbee_conTx(edcon_b18074, NULL, "IR%c", 0))                       != XBEE_ENONE) die("xbee_conTx(edcon_b18074, IR)", ret); 
  if ((ret = xbee_conTx(edcon_b18074, NULL, "IC%c", 0x02))                    != XBEE_ENONE) die("xbee_conTx(edcon_b18074, IR)", ret); // 0x02 is the mask for D1
  // Call Back edcon_b18074
  if ((ret = xbee_conNew(xbee, &cbcon_b18074, "I/O", &ed_addr_b18074))              != XBEE_ENONE) die("xbee_conNew(cbcon_b18074)", ret); 
  if ((ret = xbee_conCallbackSet(cbcon_b18074, callback_btn_b18074, NULL)) != XBEE_ENONE) die("xbee_conCallbackSet(cbcon_b18074)", ret);
 return 0;
}

void sync(){
 xbee_conTx(rocon_a238d4, NULL, "D0%c", led1_status+4);
 xbee_conTx(edcon_af5a99, NULL, "D0%c", led2_status+4);
 xbee_conTx(rocon_8b5f51, NULL, "D0%c", led3_status+4); 
}

int main(int argc, char *argv[]) {
 pthread_mutex_init (&led_mutex, NULL);
 setup("/dev/ttyUSB0");
 int nselectg, nselects;
 int get_b1Behaviour, get_b2Behaviour; 
 unsigned int status_pin;
 sync();  
        do {
               clear_screen();
               general_menu ();
               printf("select operation: ");
               scanf("%d",&nselectg);
               switch (nselectg) {
                 case 0: {
	            list_network_static();
                   usleep(5000000);
                  break;
                 }
                 case 1: {
                   led1_status=!led1_status;
                   if (led1_status+4 == 4 ) printf("Router 1 - LED OFF \n");  
                   else printf("Router 1 - LED ON \n");	
                   usleep(1000000);
                  break;
                 }
                 case 2: {
                   led2_status=!led2_status;
                   if (led2_status+4 == 4 ) printf("End Device 2 - LED OFF \n");  
                   else printf("End Device 2 - LED ON \n");
                   usleep(1000000); 
                  break;
                 }
                 case 3: {
                   led3_status=!led3_status;
                   if (led3_status+4 == 4 ) printf("Router 2 - LED OFF \n");  
                   else printf("Router 2 - LED ON \n");
                   usleep(1000000); 
                  break;
                 }
                 case 4: {
                   list_menu();
                   printf("Get into Button 1 Behaviour: ");
                   scanf("%d",&get_b1Behaviour); 
                   if ((get_b1Behaviour >=0) && (get_b1Behaviour <=7)){
                    b1Behaviour=get_b1Behaviour;
                   } 
                   else printf("error digit\n"); 
                  break;
                 }
                 case 5: {
                   list_menu();
                   printf("Get into Button 2 Behaviour: ");
                   scanf("%d",&get_b2Behaviour); 
                   if ((get_b2Behaviour >=0) && (get_b2Behaviour <=7)){
                    b2Behaviour=get_b2Behaviour;
                   } 
                   else printf("error digit\n"); 
                  break;
                 }
                 case 6: {
                   clear_screen();
                   general_menu ();
                  break;
                 }
            }
            sync();        
          //usleep(1000000); /* 1 second sleep */    
        } while (nselectg!=7);
  xbee_shutdown(xbee); 
 return 0;
}


