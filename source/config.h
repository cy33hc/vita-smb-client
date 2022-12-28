#ifndef LAUNCHER_CONFIG_H
#define LAUNCHER_CONFIG_H

#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "fs.h"

#define APP_ID "SMBCLI001"
#define DATA_PATH "ux0:data/" APP_ID
#define APP_PATH "ux0:app/" APP_ID
#define CONFIG_INI_FILE  DATA_PATH "/config.ini"

#define SMB_CLIENT_VPK_URL "https://github.com/cy33hc/vita-smb-client/releases/download/latest/smbclient.vpk"
#define SMB_CLIENT_VERSION_URL "https://github.com/cy33hc/vita-smb-client/releases/download/latest/version.txt"
#define SMB_CLIENT_VERSION_PATH APP_PATH "/version.txt"
#define SMB_CLIENT_VERSION_UPDATE_PATH DATA_PATH "/tmp/version.txt"
#define SMB_CLIENT_VPK_UPDATE_PATH DATA_PATH "/tmp/smbclient.vpk"

#define CONFIG_GLOBAL "Global"

#define CONFIG_SWAP_XO "swap_xo"

#define CONFIG_SMB_SERVER_NAME "smb_server_name"
#define CONFIG_SMB_SERVER_IP "smb_server_ip"
#define CONFIG_SMB_SERVER_PORT "smb_server_port"
#define CONFIG_SMB_SERVER_USER "smb_server_user"
#define CONFIG_SMB_SERVER_PASSWORD "smb_server_password"
#define CONFIG_SMB_SERVER_SHARE "smb_server_share"

#define CONFIG_LAST_SITE "last_site"

#define CONFIG_LOCAL_DIRECTORY "local_directory"
#define CONFIG_REMOTE_DIRECTORY "remote_directory"
#define CONFIG_UPDATE_WARN_MISSING "warn_missing_installs"

#define CONFIG_LANGUAGE "language"

struct SmbSettings
{
    char site_name[32];
    char server_ip[16];
    char username[33];
    char password[25];
    int server_port;
    char share[256];
};

extern bool swap_xo;
extern std::vector<std::string> sites;
extern std::map<std::string,SmbSettings> site_settings;
extern char local_directory[MAX_PATH_LENGTH];
extern char remote_directory[MAX_PATH_LENGTH];
extern char app_ver[6];
extern char last_site[32];
extern char display_site[32];
extern char language[128];
extern bool warn_missing_installs;
extern SmbSettings *smb_settings;

namespace CONFIG {
    void LoadConfig();
    void SaveConfig();
    void RemoveFromMultiValues(std::vector<std::string> &multi_values, std::string value);
    void ParseMultiValueString(const char* prefix_list, std::vector<std::string> &prefixes, bool toLower);
    std::string GetMultiValueString(std::vector<std::string> &multi_values);
}
#endif
