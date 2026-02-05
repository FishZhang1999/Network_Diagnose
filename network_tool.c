#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define OUTPUT_FILE "network_report.txt"
#define BUFFER_SIZE 4096

void write_header(FILE *fp) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char datetime[100];
    strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", local);
    
    fprintf(fp, "========================================\n");
    fprintf(fp, "    NETWORK DIAGNOSTIC REPORT\n");
    fprintf(fp, "    Generated: %s\n", datetime);
    fprintf(fp, "========================================\n\n");
}

void run_command(FILE *fp, const char *title, const char *command) {
    FILE *pipe;
    char buffer[BUFFER_SIZE];

    fprintf(fp, "\n----------------------------------------\n");
    fprintf(fp, "[%s]\n", title);
    fprintf(fp, "Command: %s\n", command);
    fprintf(fp, "----------------------------------------\n");

    printf("\nRunning: %s...\n", title);

    pipe = _popen(command, "r");
    if (pipe == NULL) {
        fprintf(fp, "Error: Failed to execute command\n");
        printf("Error: Failed to execute command\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        fprintf(fp, "%s", buffer);
        printf("%s", buffer);
    }

    _pclose(pipe);
    printf("\n");
}

int main(int argc, char *argv[]) {
    FILE *fp;
    char computer_name[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computer_name);
    int auto_open = 1;
    
    // Check for command line arguments
    if (argc > 1 && strcmp(argv[1], "--no-open") == 0) {
        auto_open = 0;
    }
    
    printf("Network Diagnostic Tool\n");
    printf("=======================\n\n");
    printf("Gathering network information...\n");
    printf("This may take a minute...\n\n");
    
    fp = fopen(OUTPUT_FILE, "w");
    if (fp == NULL) {
        printf("Error: Cannot create output file.\n");
        printf("Press Enter to exit...");
        getchar();
        return 1;
    }
    
    write_header(fp);
    
    // Get computer name
    if (GetComputerNameA(computer_name, &size)) {
        fprintf(fp, "Computer Name: %s\n\n", computer_name);
    }

    // Public IP Address (using curl to avoid PowerShell security warnings)
    run_command(fp, "Public IP Address", "curl -s --connect-timeout 10 https://api.ipify.org 2>nul || echo Unable to retrieve public IP");

    // Basic IP Configuration
    run_command(fp, "IP Configuration (All)", "ipconfig /all");
    
    // Network Interface Status
    run_command(fp, "Network Interface Status", "netsh interface show interface");
    
    // Routing Table
    run_command(fp, "Routing Table", "route print");
    
    // ARP Table
    run_command(fp, "ARP Table", "arp -a");
    
    // DNS Cache (limited to first 20 lines)
    //run_command(fp, "DNS Cache (Sample)", "ipconfig /displaydns | findstr /N \".\" | findstr \"^[0-9]: ^[0-9][0-9]:\" | findstr /V \"^[0-9][0-9][0-9]:\"");
    
    // Network Adapter Details (using PowerShell instead of deprecated wmic)
    run_command(fp, "Network Adapter Details", 
        "powershell -Command \"Get-NetAdapter | Select-Object Name, InterfaceDescription, Status, LinkSpeed | Format-Table -AutoSize\"");
    
    // Wireless Information (if applicable)
    run_command(fp, "Wireless Network Info", "netsh wlan show interfaces");
    
    // Winsock Catalog
    //run_command(fp, "Winsock Catalog State", "netsh winsock show catalog");
    
    // Network Connection Status
    run_command(fp, "Network Connection Status", "netsh interface ip show config");
    
    // Windows Firewall Status
    run_command(fp, "Windows Firewall Status", "netsh advfirewall show allprofiles");
    
    // Proxy Settings
    run_command(fp, "Check Proxy Settings", "netsh winhttp show proxy");    

    // DNS Test - Google
    run_command(fp, "DNS Lookup Test (google.com)", "nslookup google.com");

    // Ping Tests
    run_command(fp, "Ping Test - Loopback", "ping 127.0.0.1 -n 4");
    run_command(fp, "Ping Test - Google DNS (8.8.8.8)", "ping 8.8.8.8 -n 4");
    run_command(fp, "Ping Test - Google.com", "ping google.com -n 4");

    // Traceroute
    //run_command(fp, "Traceroute to Google.com", "tracert -d -h 15 google.com");
    
    fprintf(fp, "\n\n========================================\n");
    fprintf(fp, "    END OF NETWORK DIAGNOSTIC REPORT\n");
    fprintf(fp, "========================================\n");
    
    fclose(fp);
    
    printf("\n[OK] Network diagnostic complete!\n");
    printf("Report saved to: %s\n\n", OUTPUT_FILE);
    
    if (auto_open) {
        printf("Opening report...\n");
        
        // Open the file with notepad
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "notepad \"%s\"", OUTPUT_FILE);
        system(cmd);
    } else {
        printf("Report saved. Use --open to view it.\n");
    }
    
    return 0;
}
