#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <unistd.h>

int main(void)
{
    modbus_t *ctx;
    int rc = 0;
    int i;
    uint16_t tab_reg[64];
    uint16_t tab_src[10] = {1};

    ctx = modbus_new_tcp("192.168.100.64", 1502);

    while (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    }

    while(1)
    {   
        for(i=0; i<10; i++) {
            tab_src[i]+=i;
        }
        rc= modbus_write_registers(ctx, 0, 10, tab_src);
        // rc= modbus_read_input_registers(ctx, 0, 10, tab_reg);
        if(rc == -1) {
            fprintf(stderr, "%s\n", modbus_strerror(errno));
            break;
        }

        printf("%d\n", rc);

        // for (i=0; i < rc; i++) {
        //     printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i] );
        // }
        // printf("----------------------------------\n");
        rc= modbus_write_registers(ctx, 0, 10, tab_src);
        sleep(2);
    }

    printf("freeing up resourses\n");
    modbus_close(ctx);
    modbus_free(ctx);
    return 0;
}