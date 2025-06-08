#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// CO2 배출 계수 상수 정의
const double C = 0.2;    // 차량 이동 (kg CO2/km)
const double E = 0.424;  // 전기 사용 (kg CO2/kWh)
const double D = 0.1;    // 일회용품 (kg CO2/개)
const double G = 2.3;    // 가스 사용 (kg CO2/m³)
const double P = 0.5;    // 택배 이용 (kg CO2/회)

// 더미 데이터 생성 함수
void generate_data(const char* filename, int year, int month) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("파일을 열 수 없습니다: %s\n", filename);
        return;
    }

    for (int day = 1; day <= 28; day++) {
        // 목표 CO2 배출량 (0~10 사이 균등 분포)
        double target_co2 = ((double)rand() / RAND_MAX) * 10.0;

        // 정수형 항목 먼저 생성
        int plastic_cnt = rand() % 11;     // 0 ~ 10
        int delivery_cnt = rand() % 4;     // 0 ~ 3

        double fixed_co2 = plastic_cnt * D + delivery_cnt * P;
        double remaining_co2 = target_co2 - fixed_co2;

        // CO2 초과 시 정수 항목 조정
        while (remaining_co2 < 0.0) {
            if (plastic_cnt > 0) plastic_cnt--;
            else if (delivery_cnt > 0) delivery_cnt--;
            else break;
            fixed_co2 = plastic_cnt * D + delivery_cnt * P;
            remaining_co2 = target_co2 - fixed_co2;
        }

        // 실수형 항목 무작위 비율로 분배
        double w1 = (double)rand() / RAND_MAX;
        double w2 = (double)rand() / RAND_MAX;
        double w3 = (double)rand() / RAND_MAX;
        double sum = w1 + w2 + w3;
        w1 /= sum; w2 /= sum; w3 /= sum;

        double car_km = (w1 * remaining_co2) / C;
        double elec_kwh = (w2 * remaining_co2) / E;
        double gas_m3 = (w3 * remaining_co2) / G;

        // 값 제한 (현실 범위)
        if (car_km > 35.0) car_km = 35.0;
        if (elec_kwh > 20.0) elec_kwh = 20.0;
        if (gas_m3 > 5.0) gas_m3 = 5.0;

        // 파일에 기록
        fprintf(fp, "%d,%d,%d,%.2f,%.2f,%d,%.2f,%d\n",
            year, month, day, car_km, elec_kwh, plastic_cnt, gas_m3, delivery_cnt);
    }

    fclose(fp);
    printf("파일 생성 완료: %s\n", filename);
}

int main() {
    int year, month;
    printf("연도와 월을 입력하세요 (예: 2025 5): ");
    scanf("%d %d", &year, &month);

    srand((unsigned int)time(NULL));

    // 연월 문자열 만들기 (ex: 2505)
    char ym[5];
    snprintf(ym, sizeof(ym), "%02d%02d", year % 100, month);

    // 파일명 생성
    char filename_a[20], filename_b[20];
    snprintf(filename_a, sizeof(filename_a), "a%s.csv", ym);
    snprintf(filename_b, sizeof(filename_b), "b%s.csv", ym);

    // 두 개의 더미 데이터 파일 생성
    generate_data(filename_a, year, month);
    generate_data(filename_b, year, month);

    return 0;
}
