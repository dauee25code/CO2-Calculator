#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>  // 문자열 처리 헤더
#include <time.h>

#define MAX_DAYS 366 // 최대 1년치 데이터 저장 (윤년 고려)
#define MAX_USERS 10 // 최대 사용자 수

char user_names[MAX_USERS][30];
int user_count = 0, current_user = -1;

// 사용자별 날짜(연도, 월, 일), 차량 이동 거리[km], 전기 사용량[kWh], 일회용품 개수, 총 CO2 배출량[kg]을 저장하는 이차원 배열
int user_year[MAX_USERS][MAX_DAYS];
int user_month[MAX_USERS][MAX_DAYS];
int user_day[MAX_USERS][MAX_DAYS];
double car_km[MAX_USERS][MAX_DAYS];
double elec_kwh[MAX_USERS][MAX_DAYS];
int plastic_cnt[MAX_USERS][MAX_DAYS];
double total_co2[MAX_USERS][MAX_DAYS];
int day_count[MAX_USERS];

const double C = 0.2, E = 0.424, D = 0.1; // CO2 배출 계수 C(차량 이동 거리), E(전기 사용량), D(일회용품 개수)

void clear_input_buffer() {
    while (getchar() != '\n'); // 표준 입력으로부터 한 글자를 읽어오는 함수 getchar()
}

void login_user()
{
    printf("\n사용자 이름 입력: ");
    char name[30];
    scanf("%s", name);
    clear_input_buffer();

    // 입력된 이름과 기존 사용자 이름을 비교
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(user_names[i], name) == 0)
        {
            current_user = i;
            printf("%s 사용자로 로그인되었습니다.\n", name);
            return;
        }
    }

    // 사용자 수가 최대치에 도달하지 않은 경우에만 새 사용자 등록
    if (user_count < MAX_USERS)
    {
        strcpy(user_names[user_count], name);
        day_count[user_count] = 0;
        current_user = user_count++;
        printf("%s 사용자 등록 및 로그인 완료.\n", name);
    }
    else
    {
        printf("사용자 수 초과입니다.\n");
    }
}

void input_data()
{
    // 현재 사용자가 로그인하지 않은 경우 로그인 요청 메시지 출력
    if (current_user == -1)
    {
        printf("로그인 먼저 하세요.\n");
        return;
    }

    // 현재 사용자의 날짜 수를 가져오고, 최대 날짜 수를 초과한 경우는 오류 메시지 출력
    int idx = day_count[current_user];
    if (idx >= MAX_DAYS)
    {
        printf("더 이상 데이터를 저장할 수 없습니다.\n");
        return;
    }

    // 현재 날짜를 입력받고, 차량 이동 거리, 전기 사용량, 일회용품 개수를 입력받아 저장
    printf("날짜 입력(YYYY MM DD): ");
    scanf("%d %d %d", &user_year[current_user][idx], &user_month[current_user][idx], &user_day[current_user][idx]);
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

    // 총 배출량을 배출 계수에 따라 계산하고 소수점 둘째 자리까지 출력
    total_co2[current_user][idx] = car_km[current_user][idx] * C +
        elec_kwh[current_user][idx] * E +
        plastic_cnt[current_user][idx] * D;
    printf("오늘의 배출량: %.2f kg CO₂\n", total_co2[current_user][idx]);

    // 조언 메시지 출력
    if (car_km[current_user][idx] > 20.0)
        printf("→ 대중교통 이용을 늘리면 탄소 배출을 줄일 수 있습니다.\n");
    if (elec_kwh[current_user][idx] > 15.0)
        printf("→ 에어컨/난방기기 절전 및 조명 끄기를 실천해보세요.\n");
    if (plastic_cnt[current_user][idx] > 5)
        printf("→ 다회용품 사용으로 일회용품 사용을 줄일 수 있습니다.\n");

    day_count[current_user]++;
}

// 특정 월의 평균 탄소 배출량을 계산
double get_monthly_avg(int user, int year, int month)
{
    double sum = 0;
    int cnt = 0;
    for (int j = 0; j < day_count[user]; j++)
    {
        if (user_year[user][j] == year && user_month[user][j] == month)
        {
            sum += total_co2[user][j];
            cnt++;
        }
    }
    return cnt ? sum / cnt : 0; // 삼항 연산자, cnt가 0이 아니면 평균값을 반환, 0이면 0 반환
}

// 월평균 탄소 배출량 그래프를 출력
void show_graph()
{
    int year, month;
    printf("연도와 월을 입력하세요 (YYYY MM): ");
    scanf("%d %d", &year, &month);
    clear_input_buffer();

    printf("\n======== %d년 %d월 월평균 탄소 배출량 그래프 ========\n", year, month);
    for (int i = 0; i < user_count; i++) // 모든 사용자에 대해 반복
    {
        if (day_count[i] == 0) // 데이터가 없는 사용자는 건너뜀
        {
            printf("%-4s : 데이터 없음\n", user_names[i]);
            continue;
        }
        // 월평균 탄소 배출량을 계산하고 막대 그래프 출력
        double avg = get_monthly_avg(i, year, month);
        int bar = (int)(avg); // 평균값으로 막대 길이 결정 (단위: kg)
        printf("%-4s : ", user_names[i]);
        for (int k = 0; k < bar; k++) printf("■");
        printf(" (%.1f kg/일)\n", avg);
    }
    printf("=======================================================\n");
}

void compare_users()
{
    printf("\n======== 탄소 배출 비교 ========\n");
    printf("이름   | 누적(kg) | 평균(kg)\n");
    double min_avg = 1e9;
    int min_idx = -1;

    // 모든 사용자에 대해 누적 및 평균 탄소 배출량을 계산하고 출력
    for (int i = 0; i < user_count; i++)
    {
        double sum = 0;
        for (int j = 0; j < day_count[i]; j++)
            sum += total_co2[i][j];
        double avg = day_count[i] ? sum / day_count[i] : 0;
        printf("%-6s | %8.2f | %8.2f\n", user_names[i], sum, avg);
        if (avg < min_avg && day_count[i])
            min_avg = avg, min_idx = i;
    }

    // 누적 및 평균 탄소 배출량이 가장 적은 사용자를 출력
    if (min_idx != -1)
        printf("\n>> 탄소 배출이 가장 적은 사람은 '%s'입니다!\n", user_names[min_idx]);
    clear_input_buffer();
    printf("계속하려면 엔터를 누르세요...");
    getchar();
}

void load_from_file()
{
    if (current_user == -1)
    {
        printf("로그인 먼저 하세요.\n");
        return;
    }
    char filename[100];
    printf("파일명 입력: ");
    scanf("%s", filename);
    clear_input_buffer();

    FILE* file = fopen(filename, "r"); // 파일을 읽기 모드로 열고, 만약 파일이 없으면 오류 메시지를 출력
    if (!file)
    {
        printf("파일을 열 수 없습니다.\n");
        return;
    }
    int year, month, day, cnt;
    double km, kwh;
    while (fscanf(file, "%d,%d,%d,%lf,%lf,%d", &year, &month, &day, &km, &kwh, &cnt) == 6)
    {
        // 입력된 날짜가 현재 날짜보다 미래인 경우는 무시
        if (day_count[current_user] >= MAX_DAYS)
        {
            printf("더 이상 데이터를 저장할 수 없습니다.\n");
            break;
        }
        //연도, 월, 일, 차량 이동 거리, 전기 사용량, 일회용품 개수를 읽어와서 저장
        user_year[current_user][day_count[current_user]] = year;
        user_month[current_user][day_count[current_user]] = month;
        user_day[current_user][day_count[current_user]] = day;
        car_km[current_user][day_count[current_user]] = km;
        elec_kwh[current_user][day_count[current_user]] = kwh;
        plastic_cnt[current_user][day_count[current_user]] = cnt;
        total_co2[current_user][day_count[current_user]] = km * C + kwh * E + cnt * D;
        day_count[current_user]++; //현재 사용자에 대한 날짜 수 증가
    }
    fclose(file);
    printf("파일에서 데이터 입력 완료 (%d일)\n", day_count[current_user]);
}

void menu()
{
    printf("\n=== 탄소 발자국 프로그램 ===\n");
    time_t now; //시간 값을 나타내기 위해 변수 now를 선언
    struct tm* t; //tm 형태(연도, 월, 일, 시, 분, 초 등)의 포인터 변수 t를 선언
    time(&now); //현재 시간을 구해서 변수 now에 저장
    t = localtime(&now); //포인터 t에 초 단위 시간을 읽을 수 있는 형태로 변환 후 저장
    printf("현재 날짜: %04d-%02d-%02d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday); //tm_year는 1900년부터 경과한 연수, tm_mon는 0부터 시작하는 월
    printf("1. 로그인\n2. 입력\n3. 그래프\n4. 비교\n5. 파일에서 데이터 읽기\n6. 종료\n선택: ");
}

int main()
{
    int choice;
    while (1)
    {
        menu();

        //숫자가 아닌 값 입력시 오류 메시지를 출력
        if (scanf("%d", &choice) != 1) //scanf 반환값은 읽어들인 항목의 개수이고, 0이나 -1 반환한 경우는 매칭 실패 또는 오류를 의미함
        {
            clear_input_buffer();
            printf("잘못된 입력입니다. 숫자를 입력하세요.\n");
            continue;
        }
        clear_input_buffer();
        switch (choice)
        {
        case 1: login_user(); break;
        case 2: input_data(); break;
        case 3: show_graph(); break;
        case 4: compare_users(); break;
        case 5: load_from_file(); break;
        case 6: return 0;
        default: printf("잘못된 선택입니다.\n");
        }
    }
    return 0;
}
