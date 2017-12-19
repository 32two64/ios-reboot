s (54 sloc)  2.18 KB
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/diagnostics_relay.h>

enum cmd_mode {
        CMD_NONE = 0,
        CMD_RESTART,
};

int main(int argc, char **argv)
{
        idevice_t device = NULL;
        lockdownd_client_t lockdown_client = NULL;
        diagnostics_relay_client_t diagnostics_client = NULL;
        lockdownd_error_t ret = LOCKDOWN_E_UNKNOWN_ERROR;
        lockdownd_service_descriptor_t service = NULL;
        int result = -1;
        int i;
        const char *udid = NULL;
        int cmd = CMD_NONE;
        char* cmd_arg = NULL;
        cmd = CMD_RESTART;

        if (cmd == CMD_NONE) {
        }

        if (IDEVICE_E_SUCCESS != idevice_new(&device, udid)) {              
        }

        if (LOCKDOWN_E_SUCCESS != lockdownd_client_new_with_handshake(device, &lockdown_client, "3x7R00Tripper")) {
                idevice_free(device);
        }
        ret = lockdownd_start_service(lockdown_client, "com.apple.mobile.diagnostics_relay", &service);
        if (ret != LOCKDOWN_E_SUCCESS) {
                ret = lockdownd_start_service(lockdown_client, "com.apple.iosdiagnostics.relay", &service);
        }

        lockdownd_client_free(lockdown_client);

        if ((ret == LOCKDOWN_E_SUCCESS) && service && (service->port > 0)) {
                if (diagnostics_relay_client_new(device, service, &diagnostics_client) != DIAGNOSTICS_RELAY_E_SUCCESS) {
                        result = -1;
                } else {
                        switch (cmd) {
                                case CMD_RESTART:
                                        if (diagnostics_relay_restart(diagnostics_client, 0) == DIAGNOSTICS_RELAY_E_SUCCESS) {
                                                result = EXIT_SUCCESS;
                                        } else {
                                        }
                                break;
                        }
                        diagnostics_relay_goodbye(diagnostics_client);
                        diagnostics_relay_client_free(diagnostics_client);
                }
        } 
}
