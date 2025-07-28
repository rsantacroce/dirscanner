#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>

#define MAX_PATH 4096
#define MAX_DIRS 1000

// ANSI Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

typedef struct {
    char path[MAX_PATH];
    char name[256];
    long long size;
} DirInfo;

// Function to get human readable size
void format_size(long long bytes, char *output) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double size = (double)bytes;
    
    while (size >= 1024.0 && unit < 4) {
        size /= 1024.0;
        unit++;
    }
    
    if (unit == 0) {
        sprintf(output, "%lld %s", bytes, units[unit]);
    } else {
        sprintf(output, "%.2f %s", size, units[unit]);
    }
}

// Get color based on size
const char* get_size_color(long long size) {
    if (size > 1024LL * 1024 * 1024 * 10) return RED;      // > 10GB - Red
    if (size > 1024LL * 1024 * 1024) return MAGENTA;       // > 1GB - Magenta
    if (size > 1024LL * 1024 * 100) return YELLOW;         // > 100MB - Yellow
    if (size > 1024LL * 1024 * 10) return CYAN;            // > 10MB - Cyan
    return GREEN;                                           // < 10MB - Green
}

// Get progress bar
void get_progress_bar(long long size, long long max_size, char *bar, int width) {
    int filled = (int)((double)size / max_size * width);
    int i;
    
    strcpy(bar, "[");
    for (i = 0; i < filled; i++) {
        strcat(bar, "█");
    }
    for (i = filled; i < width; i++) {
        strcat(bar, "░");
    }
    strcat(bar, "]");
}

// Recursive function to calculate directory size
long long calculate_dir_size(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[MAX_PATH];
    long long total_size = 0;
    
    dir = opendir(path);
    if (dir == NULL) {
        return 0;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        if (lstat(full_path, &file_stat) == 0) {
            if (S_ISREG(file_stat.st_mode)) {
                total_size += file_stat.st_size;
            } else if (S_ISDIR(file_stat.st_mode)) {
                total_size += calculate_dir_size(full_path);
            }
        }
    }
    
    closedir(dir);
    return total_size;
}

// Function to scan directory
int scan_directory(const char *base_path, DirInfo dirs[], int max_dirs) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[MAX_PATH];
    int count = 0;
    
    dir = opendir(base_path);
    if (dir == NULL) {
        printf("%s✗ Cannot open directory: %s%s\n", RED, base_path, RESET);
        return 0;
    }
    
    printf("%s%s🔍 Scanning %s...%s\n", BOLD, CYAN, base_path, RESET);
    
    while ((entry = readdir(dir)) != NULL && count < max_dirs) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, entry->d_name);
        
        if (lstat(full_path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode)) {
            strncpy(dirs[count].path, full_path, MAX_PATH - 1);
            dirs[count].path[MAX_PATH - 1] = '\0';
            strncpy(dirs[count].name, entry->d_name, 255);
            dirs[count].name[255] = '\0';
            
            printf("%s  📂 Calculating size of %s...%s", BLUE, entry->d_name, RESET);
            fflush(stdout);
            
            dirs[count].size = calculate_dir_size(full_path);
            
            char size_str[50];
            format_size(dirs[count].size, size_str);
            printf(" %s%s%s\n", get_size_color(dirs[count].size), size_str, RESET);
            
            count++;
        }
    }
    
    closedir(dir);
    return count;
}

// Comparison function for qsort
int compare_dirs(const void *a, const void *b) {
    DirInfo *dir_a = (DirInfo *)a;
    DirInfo *dir_b = (DirInfo *)b;
    
    if (dir_a->size > dir_b->size) return -1;
    if (dir_a->size < dir_b->size) return 1;
    return 0;
}

// Generate HTML report
void generate_html_report(DirInfo dirs[], int count, const char *scan_path, long long total_size) {
    FILE *html_file = fopen("directory_report.html", "w");
    if (html_file == NULL) {
        printf("%sError: Cannot create HTML file%s\n", RED, RESET);
        return;
    }
    
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0'; // Remove newline
    
    fprintf(html_file, "<!DOCTYPE html>\n<html>\n<head>\n");
    fprintf(html_file, "<title>Directory Size Report</title>\n");
    fprintf(html_file, "<style>\n");
    fprintf(html_file, "body { font-family: 'Segoe UI', Arial, sans-serif; margin: 40px; background: #f5f5f5; }\n");
    fprintf(html_file, ".container { background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }\n");
    fprintf(html_file, "h1 { color: #333; border-bottom: 3px solid #4CAF50; padding-bottom: 10px; }\n");
    fprintf(html_file, "h2 { color: #666; margin-top: 30px; }\n");
    fprintf(html_file, ".summary { background: #e8f5e8; padding: 15px; border-radius: 5px; margin: 20px 0; }\n");
    fprintf(html_file, "table { width: 100%%; border-collapse: collapse; margin-top: 20px; }\n");
    fprintf(html_file, "th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    fprintf(html_file, "th { background-color: #4CAF50; color: white; }\n");
    fprintf(html_file, "tr:hover { background-color: #f5f5f5; }\n");
    fprintf(html_file, ".size-bar { width: 200px; height: 20px; background: #eee; border-radius: 10px; overflow: hidden; }\n");
    fprintf(html_file, ".size-fill { height: 100%%; background: linear-gradient(90deg, #4CAF50, #45a049); }\n");
    fprintf(html_file, ".huge { color: #d32f2f; font-weight: bold; }\n");
    fprintf(html_file, ".large { color: #f57c00; font-weight: bold; }\n");
    fprintf(html_file, ".medium { color: #1976d2; }\n");
    fprintf(html_file, ".small { color: #388e3c; }\n");
    fprintf(html_file, ".footer { margin-top: 30px; padding-top: 20px; border-top: 1px solid #ddd; color: #666; font-size: 0.9em; }\n");
    fprintf(html_file, "</style>\n</head>\n<body>\n");
    
    fprintf(html_file, "<div class='container'>\n");
    fprintf(html_file, "<h1>📊 Directory Size Report</h1>\n");
    
    char total_size_str[50];
    format_size(total_size, total_size_str);
    
    fprintf(html_file, "<div class='summary'>\n");
    fprintf(html_file, "<strong>Scan Path:</strong> %s<br>\n", scan_path);
    fprintf(html_file, "<strong>Total Directories:</strong> %d<br>\n", count);
    fprintf(html_file, "<strong>Total Size:</strong> %s<br>\n", total_size_str);
    fprintf(html_file, "<strong>Generated:</strong> %s\n", time_str);
    fprintf(html_file, "</div>\n");
    
    fprintf(html_file, "<h2>📁 Directory Breakdown</h2>\n");
    fprintf(html_file, "<table>\n");
    fprintf(html_file, "<tr><th>Rank</th><th>Directory</th><th>Size</th><th>Visual</th><th>%% of Total</th></tr>\n");
    
    long long max_size = (count > 0) ? dirs[0].size : 1;
    
    for (int i = 0; i < count; i++) {
        char size_str[50];
        format_size(dirs[i].size, size_str);
        
        double percentage = (double)dirs[i].size / total_size * 100;
        int bar_width = (int)((double)dirs[i].size / max_size * 100);
        
        const char *size_class = "small";
        if (dirs[i].size > 1024LL * 1024 * 1024 * 10) size_class = "huge";
        else if (dirs[i].size > 1024LL * 1024 * 1024) size_class = "large";
        else if (dirs[i].size > 1024LL * 1024 * 100) size_class = "medium";
        
        fprintf(html_file, "<tr>\n");
        fprintf(html_file, "<td>%d</td>\n", i + 1);
        fprintf(html_file, "<td>%s</td>\n", dirs[i].name);
        fprintf(html_file, "<td class='%s'>%s</td>\n", size_class, size_str);
        fprintf(html_file, "<td><div class='size-bar'><div class='size-fill' style='width: %d%%'></div></div></td>\n", bar_width);
        fprintf(html_file, "<td>%.1f%%</td>\n", percentage);
        fprintf(html_file, "</tr>\n");
    }
    
    fprintf(html_file, "</table>\n");
    fprintf(html_file, "<div class='footer'>Report generated by Directory Scanner</div>\n");
    fprintf(html_file, "</div>\n</body>\n</html>\n");
    
    fclose(html_file);
    printf("%s%s✅ HTML report generated: directory_report.html%s\n", BOLD, GREEN, RESET);
}

int main(int argc, char *argv[]) {
    DirInfo dirs[MAX_DIRS];
    char *scan_path;
    int generate_html = 0;
    
    // Parse command line arguments
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--html") == 0 || strcmp(argv[i], "-h") == 0) {
                generate_html = 1;
            } else {
                scan_path = argv[i];
            }
        }
    }
    
    if (scan_path == NULL) {
        struct passwd *pw = getpwuid(getuid());
        scan_path = pw->pw_dir;
    }
    
    printf("%s%s📁 Directory Size Scanner%s\n", BOLD, CYAN, RESET);
    printf("%s=================================%s\n\n", CYAN, RESET);
    
    int dir_count = scan_directory(scan_path, dirs, MAX_DIRS);
    
    if (dir_count == 0) {
        printf("%s❌ No directories found or unable to access the specified path.%s\n", RED, RESET);
        return 1;
    }
    
    qsort(dirs, dir_count, sizeof(DirInfo), compare_dirs);
    
    printf("\n%s%s📊 Directory sizes (sorted by size):%s\n", BOLD, YELLOW, RESET);
    printf("%s=====================================%s\n\n", YELLOW, RESET);
    
    long long total_size = 0;
    long long max_size = dirs[0].size;
    
    for (int i = 0; i < dir_count; i++) {
        char size_str[50];
        char progress_bar[32];
        
        format_size(dirs[i].size, size_str);
        get_progress_bar(dirs[i].size, max_size, progress_bar, 20);
        
        printf("%s%3d.%s %-30s %s%15s%s %s%s%s %.1f%%\n", 
               BOLD, i + 1, RESET,
               dirs[i].name,
               get_size_color(dirs[i].size), size_str, RESET,
               get_size_color(dirs[i].size), progress_bar, RESET,
               (double)dirs[i].size / max_size * 100);
        
        total_size += dirs[i].size;
    }
    
    char total_size_str[50];
    format_size(total_size, total_size_str);
    
    printf("\n%s%s📈 Summary:%s\n", BOLD, GREEN, RESET);
    printf("%sTotal size of %d directories: %s%s\n", GREEN, dir_count, total_size_str, RESET);
    printf("%sScanned path: %s%s\n", BLUE, scan_path, RESET);
    
    if (generate_html) {
        generate_html_report(dirs, dir_count, scan_path, total_size);
    } else {
        printf("\n%s💡 Tip: Use --html flag to generate an HTML report!%s\n", YELLOW, RESET);
    }
    
    return 0;
}
