#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_DAYS 366
#define MAX_USERS 10

char user_names[MAX_USERS][30];
int user_count = 0, current_user = -1;

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

const double C = 0.2;
const double E = 0.424;
const double D = 0.1;
const double G = 2.3;
const double P = 0.5;

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

void input_data() {
    if (current_user == -1) {
        printf("로그인 먼저 하세요.\n");
        return;
    }

    int idx = day_count[current_user];
    if (idx >= MAX_DAYS) {
        printf("더 이상 데이터를 저장할 수 없습니다.\n");
        return;
    }

    printf("날짜 입력(YYYY MM DD): ");
    scanf("%d %d %d",
        &user_year[current_user][idx],
        &user_month[current_user][idx],
        &user_day[current_user][idx]);
    clear_input_buffer();

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

    total_co2[current_user][idx] =
        car_km[current_user][idx] * C +
        elec_kwh[current_user][idx] * E +
        plastic_cnt[current_user][idx] * D +
        gas_m3[current_user][idx] * G +
        delivery_cnt[current_user][idx] * P;

    printf("오늘의 배출량: %.2f kg CO₂\n", total_co2[current_user][idx]);

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

double get_monthly_avg(int user, int year, int month) {
    double sum = 0;
    int cnt = 0;
    for (int j = 0; j < day_count[user]; j++) {
        if (user_year[user][j] == year && user_month[user][j] == month) {
            sum += total_co2[user][j];
            cnt++;
        }
    }
    return cnt ? sum / cnt : 0;
}

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

    FILE* file = fopen(filename, "r");
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

// 여러 사용자의 연도별 월별 평균 탄소배출량 출력
void show_yearly_monthly_avg_all(int year) {
    printf("\n===== %d년 월별 평균 탄소 배출량 =====\n", year);
    // 월 헤더 출력
    printf("%-8s", "사용자");
    for (int month = 1; month <= 12; month++)
        printf("%6d월", month);
    printf("\n");

    for (int i = 0; i < user_count; i++) {
        printf("%-8s", user_names[i]);
        for (int month = 1; month <= 12; month++) {
            double sum = 0;
            int cnt = 0;
            for (int j = 0; j < day_count[i]; j++) {
                if (user_year[i][j] == year && user_month[i][j] == month) {
                    sum += total_co2[i][j];
                    cnt++;
                }
            }
            double avg = cnt ? sum / cnt : 0;
            printf("%8.2f", avg);
        }
        printf("\n");
    }
    printf("=================================\n");
}

void yearly_monthly_avg() {
    int year;
    printf("연도를 입력하세요 (YYYY): ");
    scanf("%d", &year);
    clear_input_buffer();
    show_yearly_monthly_avg_all(year);
}

void menu() {
    printf("\n=== 탄소 발자국 프로그램 ===\n");
    time_t now;
    struct tm* t;
    time(&now);
    t = localtime(&now);
    printf("현재 날짜: %04d-%02d-%02d\n",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    printf("1. 로그인\n2. 입력\n3. 월평균 막대그래프\n4. 연도별 월별 평균\n5. 비교\n6. 파일에서 데이터 읽기\n7. 종료\n선택: ");
}

int main() {
    int choice;
    while (1) {
        menu();
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
        case 4: yearly_monthly_avg(); break;
        case 5: compare_users(); break;
        case 6: load_from_file(); break;
        case 7: return 0;
        default: printf("잘못된 선택입니다.\n");
        }
    }
    return 0;
}
