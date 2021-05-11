#include <Settings.h>

#define PARSER_RESPONSE "Response"

typedef enum {
  logged_in,
  logged_out,
  no_admin_rights,
  cmd_error,
  system_status,
  system_info,
  scan_wifi,
  save_settings,
  delete_settings,
  reload_settings,  // <---
  reset,
  reboot,  
  add_wifi,
  settings_cur,
  settings_new,
  saved_networks,
  fota_update,
  delete_networks,
  delete_wifi,
  format_fs,
  settings_modified
} mqtt_command_t;

mqtt_command_t decodeCommand(char *json, app_settings_t &settings);
