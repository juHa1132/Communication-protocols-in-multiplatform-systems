#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <curl/curl.h>

#define TRIG 4  // GPIO 23 (wiringPi pin 4)
#define ECHO 5  // GPIO 24 (wiringPi pin 5)

void measure_distance(float *distances, int num_measurements) {
    int i;
    for (i = 0; i < num_measurements; i++) {
        digitalWrite(TRIG, LOW);
        usleep(2000000);  // sleep for 2 seconds
        
        digitalWrite(TRIG, HIGH);
        usleep(10);  // sleep for 10 microseconds
        digitalWrite(TRIG, LOW);
        
        while (digitalRead(ECHO) == LOW);
        long startTime = micros();
        
        while (digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;
        
        float distance = travelTime / 58.0;
        distances[i] = distance;
        printf("Distance: %.2f cm\n", distance);
    }
}

void write_to_file(float *distances, int num_measurements, const char *file_name) {
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < num_measurements; i++) {
        fprintf(file, "%.2f cm\n", distances[i]);
    }
    
    fclose(file);
}

void upload_file_to_ftp(const char *file_name, const char *ftp_url, const char *ftp_user, const char *ftp_password) {
    CURL *curl;
    CURLcode res;
    FILE *file;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (curl) {
        file = fopen(file_name, "rb");
        if (!file) {
            perror("Failed to open file for reading");
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            exit(EXIT_FAILURE);
        }
        
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, ftp_url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, ftp_user);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, ftp_password);
        curl_easy_setopt(curl, CURLOPT_READDATA, file);
        
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        
        fclose(file);
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
}

int main() {
    if (wiringPiSetup() == -1) {
        perror("wiringPiSetup failed");
        return 1;
    }
    
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    
    int num_measurements = 20;
    float distances[num_measurements];
    
    measure_distance(distances, num_measurements);
    
    const char *file_name = "distance_measurements.txt";
    write_to_file(distances, num_measurements, file_name);
    
    const char *ftp_url = "ftp://192.168.100.45/distance_measurements.txt";
    const char *ftp_user = "ftp-user";
    const char *ftp_password = "wimirAGH24";
    
    upload_file_to_ftp(file_name, ftp_url, ftp_user, ftp_password);
    
    return 0;
}
