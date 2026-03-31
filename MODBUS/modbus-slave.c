#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <unistd.h>

const int NB_BITS = 16;
const int NB_INPUT_BITS = 16;
const int NB_REGISTERS = 16;
const int NB_INPUT_REGISTERS = 16;

const uint8_t UT_INPUT_BITS_TAB[] = {0xFF, 0xDB};

int main(void){
int s = -1;
int rc = 0;
int i;
uint8_t buf[MODBUS_TCP_MAX_ADU_LENGTH] = {};
modbus_t *ctx;
modbus_mapping_t *map;

ctx = modbus_new_tcp(NULL, 1502);

map =  modbus_mapping_new_start_address(0, NB_BITS, 0, NB_INPUT_BITS, 0, NB_REGISTERS, 0, NB_INPUT_REGISTERS);

if(map == NULL) {
    fprintf(stderr, "Failed to allocate the mapping: %s\n", modbus_strerror(errno));
    return -1;
}
else {
    printf("mapping created\n");
    printf("Size od holding registers is %ld \n", sizeof(map->tab_input_registers));

}

map->tab_input_registers[0] = 20;
map->tab_input_registers[1] = 22;
map->tab_input_registers[2] = 222;

modbus_set_bits_from_bytes(
    map->tab_input_bits, 0, NB_INPUT_REGISTERS, UT_INPUT_BITS_TAB);

for(i=0; i < NB_INPUT_REGISTERS; i++) {
    printf("Discrete Input: %d\n", map->tab_input_bits[i]);
}

while(1) {
    s = modbus_tcp_listen(ctx, 1);
    modbus_tcp_accept(ctx, &s);

    if(s < 0) {
        printf("Error listening for modbus, err %s\n", modbus_strerror(errno));
        usleep(100000);
        continue;
    } else  {
        break;
    }
}

struct mb_tcp {
    uint16_t transact;
    uint16_t protocol;
    uint16_t length;
    uint8_t unit;
    uint8_t func;
    uint8_t data[];
};

for(;;){
    do{
        rc=modbus_receive(ctx, buf);
    }while (rc == 0);

    if(rc < 0) {
        printf("%d\n", rc);
        printf("Error %s\n", modbus_strerror(errno));
        close(s);
        break;
    }

    printf("Recived poll from Master\n");
    struct mb_tcp *frame = (struct mb_tcp *)buf;
    printf("Function code is: %d\n", frame->func);
    printf("Protocol is: %d\n", frame->protocol);
    printf("Transaction is: %d\n", frame->transact);
    printf("--------------------------------\n");

     printf("Holding registers values:\n");
        for (i = 0; i < NB_REGISTERS; i++) {
            printf("Register %d: %d\n", i, map->tab_registers[i]);
        }
        printf("--------------------------------\n");


    rc = modbus_reply(ctx, buf, rc, map);
    if (rc < 0) {
        printf("%d%d\n", rc, rc);
        printf("Error %s\n", modbus_strerror(errno));
        close(s);
        break;
    }
}

close(s);
modbus_free(ctx);
modbus_mapping_free(map);

}
