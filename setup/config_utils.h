#ifndef __CONFIG_UTILS_H__
#define __CONFIG_UTILS_H__

void set_default_config(UnikeyMainSetupOptions* opt);
void read_config(UnikeyMainSetupOptions* opt);
void write_config(UnikeyMainSetupOptions* opt);
int force_engine_to_reload_config();

#endif

