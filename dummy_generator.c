#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const double C = 0.2;    // 차량 이동 (kg CO2/km)
const double E = 0.424;  // 전기 사용 (kg CO2/kWh)
const double D = 0.1;    // 일회용품 (kg CO2/개)
const double G = 2.3;    // 가스 사용 (kg CO2/m³)
const double P = 0.5;    // 택배 이용 (kg CO2/회)

int main() {
    int year;
    printf("연도를 입력하세요 (예: 2025): ");
    scanf("%d", &year);

    srand((unsigned int)time(NULL));

    // 알파벳 배열 (a, b, c, d)
    char prefix[] = { 'a', 'b', 'c', 'd' };

    // 4개의 파일 생성
    for (int i = 0; i < 4; i++) {
        char filename[20];
        snprintf(filename, sizeof(filename), "%c%02d.csv", prefix[i], year % 100);

        FILE* fp = fopen(filename, "w");
        if (!fp) {
            printf("파일을 열 수 없습니다: %s\n", filename);
            continue;
        }

        for (int month = 1; month <= 12; month++) {
            for (int day = 1; day <= 28; day++) {
                double target_co2 = ((double)rand() / RAND_MAX) * 10.0;

                int plastic_cnt = rand() % 11;     // 0 ~ 10
                int delivery_cnt = rand() % 4;     // 0 ~ 3

                double fixed_co2 = plastic_cnt * D + delivery_cnt * P;
                double remaining_co2 = target_co2 - fixed_co2;

                while (remaining_co2 < 0.0) {
                    if (plastic_cnt > 0) plastic_cnt--;
                    else if (delivery_cnt > 0) delivery_cnt--;
                    else break;
                    fixed_co2 = plastic_cnt * D + delivery_cnt * P;
                    remaining_co2 = target_co2 - fixed_co2;
                }

                double w1 = (double)rand() / RAND_MAX;
                double w2 = (double)rand() / RAND_MAX;
                double w3 = (double)rand() / RAND_MAX;
                double sum = w1 + w2 + w3;
                w1 /= sum; w2 /= sum; w3 /= sum;

                double car_km = (w1 * remaining_co2) / C;
                double elec_kwh = (w2 * remaining_co2) / E;
                double gas_m3 = (w3 * remaining_co2) / G;

                if (car_km > 35.0) car_km = 35.0;
                if (elec_kwh > 20.0) elec_kwh = 20.0;
                if (gas_m3 > 5.0) gas_m3 = 5.0;

                fprintf(fp, "%d,%d,%d,%.2f,%.2f,%d,%.2f,%d\n",
                    year, month, day, car_km, elec_kwh, plastic_cnt, gas_m3, delivery_cnt);
            }
        }

        fclose(fp);
        printf("1년치 데이터 파일 생성 완료: %s\n", filename);
    }

    return 0;
}
