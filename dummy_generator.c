#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int year, month;
    printf("연도와 월을 입력하세요 (예: 2025 5): ");
    scanf_s("%d %d", &year, &month);

    FILE* fp = fopen("data.csv", "w");
    if (!fp) {
        printf("파일을 쓸 수 없습니다.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));
    for (int day = 1; day <= 28; day++) {
        double car_km = (rand() % 3501) / 100.0;         // 0.0 ~ 35.0 km
        double elec_kwh = (rand() % 2001) / 100.0;       // 0.0 ~ 20.0 kWh
        int plastic_cnt = rand() % 11;                   // 0 ~ 10 개
        double gas_m3 = (rand() % 501) / 100.0;          // 0.0 ~ 5.0 m³
        int delivery_cnt = rand() % 4;                   // 0 ~ 3 회

        fprintf(fp, "%d,%d,%d,%.2f,%.2f,%d,%.2f,%d\n",
            year, month, day, car_km, elec_kwh, plastic_cnt, gas_m3, delivery_cnt);
    }
    fclose(fp);
    printf("data.csv 파일이 생성되었습니다.\n");
    return 0;
}
