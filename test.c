#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>  // 문자열 처리 헤더
#include <time.h>

#define MAX_DAYS 366 // 최대 1년치 데이터 저장 (윤년 고려)
#define MAX_USERS 10 // 최대 사용자 수

char user_names[MAX_USERS][30];
int user_count = 0, current_user = -1;

// 사용자별 날짜(연도, 월, 일), 차량 이동 거리[km], 전기 사용량[kWh], 가스 사용량[m³], 택배 이용 횟수
// 일회용품 개수, 가스 사용량[m³], 택배 이용 횟수, 총 CO2 배출량[kg] 저장
int user_year[MAX_USERS][MAX_DAYS];
int user_month[MAX_USERS][MAX_DAYS];
int user_day[MAX_USERS][MAX_DAYS];
double car_km[MAX_USERS][MAX_DAYS];
double elec_kwh[MAX_USERS][MAX_DAYS];
int plastic_cnt[MAX_USERS][MAX_DAYS];
double gas_m3[MAX_USERS][MAX_DAYS];
int delivery_cnt[MAX_USERS][MAX_DAYS];
double total_co2[MAX_USERS][MAX_DAYS];
int day_count[MAX_USERS];

// CO₂ 배출 계수
const double C = 0.2;    // 차량 이동 (kg CO2/km)
const double E = 0.424;  // 전기 사용 (kg CO2/kWh)
const double D = 0.1;    // 일회용품 (kg CO2/개)
const double G = 2.3;    // 가스 사용 (kg CO2/m³)
const double P = 0.5;    // 택배 이용 (kg CO2/회)

void clear_input_buffer() {
    while (getchar() != '\n');
}

void login_user() {
    printf("\n사용자 이름 입력: ");
    char name[30];
    scanf("%s", name);
    clear_input_buffer();

    for (int i = 0; i < user_count; i++) {
        if (strcmp(user_names[i], name) == 0) {
            current_user = i;
            printf("%s 사용자로 로그인되었습니다.\n", name);
            return;
        }
    }

    // 최대 사용자 수를 초과하지 않도록 확인
    if (user_count < MAX_USERS) {
        strcpy(user_names[user_count], name);
        day_count[user_count] = 0;
        current_user = user_count++;
        printf("%s 사용자 등록 및 로그인 완료.\n", name);
    }
    else {
        printf("사용자 수 초과입니다.\n");
    }
}

// 로그인된 사용자의 데이터를 입력받는 함수
void input_data() {
    if (current_user == -1) {
        printf("로그인 먼저 하세요.\n");
        return;
    }

    int idx = day_count[current_user];
    // 미래 날짜는 입력받지 않음
    if (idx >= MAX_DAYS) {
        printf("더 이상 데이터를 저장할 수 없습니다.\n");
        return;
    }

    // 날짜 입력
    printf("날짜 입력(YYYY MM DD): ");
    scanf("%d %d %d",
        &user_year[current_user][idx],
        &user_month[current_user][idx],
        &user_day[current_user][idx]);
    clear_input_buffer();

    // 차량 이동거리, 전기 사용량, 일회용품 개수, 가스 사용량, 택배 이용 횟수 입력
    printf("차량 이동 거리(km): ");
    scanf("%lf", &car_km[current_user][idx]);
    clear_input_buffer();

    printf("전기 사용량(kWh): ");
    scanf("%lf", &elec_kwh[current_user][idx]);
    clear_input_buffer();

    printf("일회용품 개수: ");
    scanf("%d", &plastic_cnt[current_user][idx]);
    clear_input_buffer();

    printf("가스 사용량(m³): ");
    scanf("%lf", &gas_m3[current_user][idx]);
    clear_input_buffer();

    printf("택배 이용 횟수: ");
    scanf("%d", &delivery_cnt[current_user][idx]);
    clear_input_buffer();

    // 총 CO₂ 배출량 계산
    total_co2[current_user][idx] =
        car_km[current_user][idx] * C +
        elec_kwh[current_user][idx] * E +
        plastic_cnt[current_user][idx] * D +
        gas_m3[current_user][idx] * G +
        delivery_cnt[current_user][idx] * P;

    printf("오늘의 배출량: %.2f kg CO₂\n", total_co2[current_user][idx]);

    // 조언 메시지
    if (car_km[current_user][idx] > 20.0)
        printf("→ 대중교통 이용을 늘리면 탄소 배출을 줄일 수 있습니다.\n");
    if (elec_kwh[current_user][idx] > 15.0)
        printf("→ 에어컨/난방기기 절전 및 조명 끄기를 실천해보세요.\n");
    if (plastic_cnt[current_user][idx] > 5)
        printf("→ 다회용품 사용으로 일회용품 사용을 줄일 수 있습니다.\n");
    if (gas_m3[current_user][idx] > 5.0)
        printf("→ 가스 절약을 위해 난방 온도를 낮추거나 요리 시간을 단축해보세요.\n");
    if (delivery_cnt[current_user][idx] > 2)
        printf("→ 합배송으로 택배 횟수를 줄여보세요.\n");

    day_count[current_user]++;
}

// 월평균 탄소 배출량 계산
double get_monthly_avg(int user, int year, int month) {
    double sum = 0;
    int cnt = 0;
    for (int j = 0; j < day_count[user]; j++) {
        if (user_year[user][j] == year && user_month[user][j] == month) {
            sum += total_co2[user][j];
            cnt++;
        }
    }
    return cnt ? sum / cnt : 0; // 삼항 연산자로 cnt가 0일 때 0을 반환하고, 아니면 평균을 반환
}

// 월평균 탄소 배출량을 막대 그래프화 하여 보여줌
void show_graph() {
    int year, month;
    printf("연도와 월을 입력하세요 (YYYY MM): ");
    scanf("%d %d", &year, &month);
    clear_input_buffer();

    printf("\n======== %d년 %d월 월평균 탄소 배출량 그래프 ========\n", year, month);
    for (int i = 0; i < user_count; i++) {
        if (day_count[i] == 0) {
            printf("%-4s : 데이터 없음\n", user_names[i]);
            continue;
        }
        double avg = get_monthly_avg(i, year, month);
        int bar = (int)(avg);
        printf("%-4s : ", user_names[i]);
        for (int k = 0; k < bar; k++) printf("■");
        printf(" (%.1f kg/일)\n", avg);
    }
    printf("=======================================================\n");
}


// 사용자별 누적 및 평균 탄소 배출량을 비교하여 가장 적은 사람을 찾음
void compare_users() {
    printf("\n======== 탄소 배출 비교 ========\n");
    printf("이름   | 누적(kg) | 평균(kg)\n");
    double min_avg = 1e9;
    int min_idx = -1;

    for (int i = 0; i < user_count; i++) {
        double sum = 0;
        for (int j = 0; j < day_count[i]; j++)
            sum += total_co2[i][j];
        double avg = day_count[i] ? sum / day_count[i] : 0;
        printf("%-6s | %8.2f | %8.2f\n",
            user_names[i], sum, avg);
        if (avg < min_avg && day_count[i]) {
            min_avg = avg;
            min_idx = i;
        }
    }

    if (min_idx != -1)
        printf("\n>> 탄소 배출이 가장 적은 사람은 '%s'입니다!\n",
            user_names[min_idx]);
    clear_input_buffer();
    printf("계속하려면 엔터를 누르세요...");
    getchar();
}

void load_from_file() {
    if (current_user == -1) {
        printf("로그인 먼저 하세요.\n");
        return;
    }
    char filename[100];
    printf("파일명 입력: ");
    scanf("%s", filename);
    clear_input_buffer();

    FILE* file = fopen(filename, "r"); // 파일을 읽기 모드로 열고 파일이 없는 경우 오류 메시지 출력
    if (!file) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    int year, month, day;
    double km, kwh, gas;
    int cnt_plastic, cnt_delivery;
    while (fscanf(file, "%d,%d,%d,%lf,%lf,%d,%lf,%d",
        &year, &month, &day,
        &km, &kwh, &cnt_plastic,
        &gas, &cnt_delivery) == 8) {
        if (day_count[current_user] >= MAX_DAYS) {
            printf("더 이상 데이터를 저장할 수 없습니다.\n");
            break;
        }
        // 날짜, 차량 이동 거리, 전기 사용량, 일회용품 개수, 가스 사용량, 택배 이용 횟수 입력
        user_year[current_user][day_count[current_user]] = year;
        user_month[current_user][day_count[current_user]] = month;
        user_day[current_user][day_count[current_user]] = day;
        car_km[current_user][day_count[current_user]] = km;
        elec_kwh[current_user][day_count[current_user]] = kwh;
        plastic_cnt[current_user][day_count[current_user]] = cnt_plastic;
        gas_m3[current_user][day_count[current_user]] = gas;
        delivery_cnt[current_user][day_count[current_user]] = cnt_delivery;
        total_co2[current_user][day_count[current_user]] =
            km * C + kwh * E + cnt_plastic * D +
            gas * G + cnt_delivery * P;
        day_count[current_user]++;
    }
    fclose(file);
    printf("파일에서 데이터 입력 완료 (%d일)\n", day_count[current_user]);
}

void menu() {
    printf("\n=== 탄소 발자국 프로그램 ===\n");
    time_t now;
    struct tm* t;
    time(&now);
    t = localtime(&now);
    printf("현재 날짜: %04d-%02d-%02d\n",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    printf("1. 로그인\n2. 입력\n3. 그래프\n4. 비교\n5. 파일에서 데이터 읽기\n6. 종료\n선택: ");
}

int main() {
    int choice;
    while (1) {
        menu();
        // scanf 함수의 반환값은 입력한 항목의 개수(정상적인 경우 1)이고, 0이나 -1(EOF)이 반환되면 입력 오류를 의미함
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("잘못된 입력입니다. 숫자를 입력하세요.\n");
            continue;
        }
        clear_input_buffer();
        switch (choice) {
        case 1: login_user();   break;
        case 2: input_data();   break;
        case 3: show_graph();   break;
        case 4: compare_users(); break;
        case 5: load_from_file(); break;
        case 6: return 0;
        default: printf("잘못된 선택입니다.\n");
        }
    }
    return 0;
}
