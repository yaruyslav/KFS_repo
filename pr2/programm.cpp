#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <iomanip>
#include <fstream>
#include <math.h>
using namespace std;
const int size = 20;
#include "prog_clases.h"
#include <thread>
#include <chrono>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
using namespace std::this_thread;
using namespace std::chrono;


int get_format_time(time_t curr_t,char format) {
    struct tm *f_Time = localtime(&curr_t);
    if (format=='y') {
        return ((int)(f_Time->tm_year)+1900);
    } else if (format=='m') {
        return ((int)(f_Time->tm_mon)+1);
    } else if (format=='d') {
        return ((int)(f_Time->tm_mday));
    } else if (format=='h') {
        return ((int)(f_Time->tm_hour));
    } else if (format=='m') {
        return ((int)(f_Time->tm_min));
    } else if (format=='s') {
        return ((int)(f_Time->tm_sec));
    }
    return 0;
}

void initialiser(Init_saved_file *inpf,Lich_data *outf1,Lichilniks_data_file *outf2) {
    for (int i=0; inpf->is_exist() && i<size; i++) {
        NameList obj = inpf->get_lich_info();
        outf1[i] = Lich_data(obj.name_number,obj.serial_number);
    }
    if (!outf2->initial_fdata(inpf)) {
        outf2->update_data();
        for (int i=0; inpf->is_exist() && i<size; i++) {
            NameList obj = inpf->get_lich_info();
            DataList obj2 = outf2->search_data(obj);
            outf1[i].set_all_cons(obj2.consumption);
        }
    }
}

bool inputAvailable() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}


void start_funk() {
    // класи, які виражають файли
    Init_calcul_file *incalc_file = new Init_calcul_file("init-calcul_data.txt");
    Init_saved_file *insave_file = new Init_saved_file("init-saved_data.csv");
    Payment_history *paymt_hist = new Payment_history("payment_history.csv");
    Lichilniks_data_file *lichd_file = new Lichilniks_data_file("lichilniks_data.csv");
    Emulator *emuu = new Emulator("lichilniks_data.csv");

    // масиви, які виражають та зберігають дані з файлів
    double *checks = new double[size];
    Lich_data *lich_data = new Lich_data[size];                     // зберігається назви та дані, які оновлюються раз в день
    InitDataList tariffes, cheats;

    // ініціалізація
    initialiser(insave_file,lich_data,lichd_file);
    tariffes = incalc_file->get_tariff();
    cheats = incalc_file->get_cheats();

    // обчислення даних
    int lt_day_obs;
    lt_day_obs = time(NULL);
    int lt_year_obs = lt_day_obs;
    int state;
    state = 0;
    cout << "Просто введіть число в будь-який момент часу:\n";
    cout << "\t0. Вихід;\n\t1. Поточний тариф та накрутку;\n\t2. Поточні дані лічильників;\n\t3. Дані чеків;\n";
    cout << "# " << "Програму запущено!\n";
    while (true) {
        InitDataList curr_tar = incalc_file->get_tariff();
        if (curr_tar.day!=tariffes.day||curr_tar.night!=tariffes.night) {
            string pack = to_string(get_format_time(time(0),'d'))+"-"+to_string(get_format_time(time(0),'m'))+"-"+to_string(get_format_time(time(0),'y'));
            insave_file->add_new_tariff(pack,curr_tar.day,curr_tar.night);
            tariffes = InitDataList(curr_tar.day,curr_tar.night);
            cout << "# " << "Тариф було змінено!\n";
        }
        
        int cr_day_obs;
        cr_day_obs = time(0);
        if (state!=1 && cr_day_obs-lt_day_obs > 6) {            // перевірка на години
            emuu->emulate_work();
            lichd_file->update_data();
            for (int i=0; i<size; i++) {
                DataList obj = lichd_file->search_data(lich_data[i].get_name());
                double curr_day_cons;
                curr_day_cons = obj.consumption - lich_data[i].get_all_cons();
                if (curr_day_cons<lich_data[i].get_day_cons()) {
                    curr_day_cons += cheats.day;
                }
                checks[i] = curr_day_cons * curr_tar.day;
                lich_data[i].set_day_cons(curr_day_cons);
                lich_data[i].set_all_cons(obj.consumption);
            }
            cout << "# День пройшов;\n";
            state = 1;
        }

        if (state!=2 && cr_day_obs-lt_day_obs > 9) {            // перевірка на години
            emuu->emulate_work();
            lichd_file->update_data();
            for (int i=0; i<size; i++) {
                DataList obj = lichd_file->search_data(lich_data[i].get_name());
                double curr_night_cons;
                curr_night_cons = obj.consumption - lich_data[i].get_all_cons();
                if (curr_night_cons<lich_data[i].get_night_cons()) {
                    curr_night_cons += cheats.night;
                }
                checks[i] += curr_night_cons * curr_tar.night;
                lich_data[i].set_night_cons(curr_night_cons);
                lich_data[i].set_all_cons(obj.consumption);
            }
            cout << "# Ніч пройшла;\n";
            state = 2;
        }

        if (state==2 && cr_day_obs-lt_day_obs > 9) {            // перевірка на години
            int ltime_arr[3] = {get_format_time(lt_day_obs,'d'),get_format_time(lt_day_obs,'m'),get_format_time(lt_day_obs,'y')};
            int ctime_arr[3] = {get_format_time(cr_day_obs,'d'),get_format_time(cr_day_obs,'m'),get_format_time(cr_day_obs,'y')};
            paymt_hist->add_payment(lich_data,checks,ltime_arr,ctime_arr);
            for (int i=0; i<size; i++) {
                checks[i] = 0;
            }
            lt_day_obs = cr_day_obs;
            state = 0;
            cout << "# День змінився;\n";
        }

        if (inputAvailable()) {
            string inp;
            getline(cin,inp);
            if (inp=="0") {
                cout << "Ви ввели: " << inp << "\n";
                break;
            } else if (inp=="1") {
                cout << "Ви ввели: " << inp << "\n";
                cout << "     Тарифи:  денний: " << curr_tar.day << ", нічний: " << curr_tar.night << ";\n";
                cout << "     Накрутки:  денна: " << cheats.day << ", нічна: " << cheats.night << ";\n";
            } else if (inp=="2") {
                cout << "Ви ввели: " << inp << "\n";
                DataList lichilnik;
                lichd_file->update_data();
                for (int i=0; i<size; i++) {
                    lichilnik = lichd_file->search_data(lich_data[i].get_name());
                    cout << setfill('0') << setw(2) << i+1 << "| name: " << lichilnik.name_number;
                    cout << ", serial_num: " << lichilnik.serial_number << ", consumption: " << lichilnik.consumption << "\n";
                }
            } else if (inp=="3") {
                cout << "Ви ввели: " << inp << "\n";
                cout << "Введіть дату початку чеку у форматі d.m.y: ";
                string date;
                cin >> date;
                paymt_hist->set_date(date);
                for (int i=0; paymt_hist->is_exist(); i++) {
                    DataList obj = paymt_hist->get_next_data();
                    cout << setfill('0') << setw(2) << i+1 << "| name_num: " << obj.name_number;
                    cout << ", serial_num: " << obj.serial_number << ", money: " << obj.consumption << "\n";
                }
            }
        }

        sleep_for(milliseconds(100));
    }
    printf("Вихід...\n");
}

int main() {
    start_funk();
    return 0;
}
