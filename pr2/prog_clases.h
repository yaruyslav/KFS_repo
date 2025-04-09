extern const int size;

class Reader {
    ifstream file_r;
    public:
    Reader(string fname) {
        file_r.open(fname);
        if (!file_r) {
            cerr << "Помилка відкриття файлу: " << fname << "\n";
        }
    }
    string get_string() {
        string line;
        if (getline(file_r,line))
            return line;
        return "-";
    }
    bool is_end() {
        if (file_r.peek()==EOF)
            return false;
        return true;
    }
    ~Reader() {
        if (file_r.is_open())
            file_r.close();
    }
};
class Rewriter {
    ofstream file_w;
    public:
    Rewriter(string fname) {
        file_w.open(fname);
        if (!file_w) {
            cerr << "Помилка відкриття файлу: " << fname << "\n";
        }
    }
    bool set_string_end(string str) {
        if (file_w.is_open()) {
            file_w << str << "\n";
            return true;
        }
        return false;
    }
    ~Rewriter() {
        if (file_w.is_open())
            file_w.close();
    }
};
class Endwriter {
    ofstream file_w;
    public:
    Endwriter(string fname) {
        file_w.open(fname,ios::app);
        if (!file_w) {
            cerr << "Помилка відкриття файлу: " << fname << "\n";
        }
    }
    bool set_string_end(string str) {
        if (file_w.is_open()) {
            file_w << str << "\n";
            return true;
        }
        return false;
    }
    ~Endwriter() {
        if (file_w.is_open())
            file_w.close();
    }
};


class InitDataList {
    public:
    InitDataList() {
        day = 0;
        night = 0;
    }
    InitDataList(double d,double n) {
        day = d;
        night = n;
    }
    double day;
    double night;
};
class NameList {
    public:
    NameList() {
        name_number = 0;
        serial_number = 0;
    }
    NameList(int name,int serial) {
        name_number = name;
        serial_number = serial;
    }
    int name_number;
    int serial_number;
};
class DataList: public NameList {
    public:
    DataList(): NameList() {
        consumption = 0;
    }
    DataList(int name,int serial,double cons): NameList(name, serial) {
        consumption = cons;
    }
    double consumption;
};
class Lich_data {
    NameList name;
    double day_cons;
    double night_cons;
    double all_cons;
    public:
    Lich_data() {
        name = NameList(0,0);
        day_cons = 0;
        night_cons = 0;
        all_cons = 0;
    }
    Lich_data(int num,int serial) {
        name = NameList(num,serial);
        day_cons = 0;
        night_cons = 0;
        all_cons = 0;
    }
    NameList get_name() {
        return name;
    }
    void set_day_cons(double number) {
        day_cons = number;
    }
    void set_night_cons(double number) {
        night_cons = number;
    }
    void set_all_cons(double number) {
        all_cons = number;
    }
    double get_day_cons() {
        return day_cons;
    }
    double get_night_cons() {
        return night_cons;
    }
    double get_all_cons() {
        return all_cons;
    }
};


class Init_calcul_file {
    string file_name;
    string lower_case(string str) {
        string out_data;
        for (int i=0; i<str.length(); i++) {
            out_data += tolower(str[i]);
        }
        return out_data;
    };
    InitDataList data_searcher(string comparator) {
        int size;
        size = 0;
        Reader *file;
        file = new Reader(file_name);
        while (file->is_end()) {
            file->get_string();
            size++;
        }
        delete(file);
        file = new Reader(file_name);
        double day_data, night_data;
        day_data = 0, night_data = 0;
        int i;
        for (i=0; i<size; i++) {
            string line = file->get_string();
            string trigger;
            for (int j=0; j<line.length(); j++) {
                trigger += line[j];
            }
            if (lower_case(trigger)==comparator) {
                int end_num;
                end_num = 0;
                for (; i<size; i++) {
                    string line = file->get_string();
                    string step1, step2;
                    int num_step, k;
                    for (num_step=1,k=0; k<line.length(); k++) {
                        if (num_step==1) {
                            if (line[k]!=':') {
                                step1 += tolower(line[k]);
                            } else {
                                num_step = 2;
                            }
                        } else if (num_step==2) {
                            step2 += tolower(line[k]);
                        } else {
                            cout << "Помилка у визначенні тарифу!\n";
                        }
                    }
                    if (step2.length()) {
                        if (step1=="day") {
                            day_data = stod(step2);
                            ++end_num;
                        } else if (step1=="night") {
                            night_data = stod(step2);
                            end_num++;
                        }
                        if (end_num==2) {
                            break;
                        }
                    }
                }
            }
        }
        delete(file);
        return InitDataList(day_data,night_data);
    }
    void data_changer(string comparator,InitDataList obj) {
        int size;
        size = 0;
        Reader *file;
        file = new Reader(file_name);
        while (file->is_end()) {
            file->get_string();
            size++;
        }
        delete(file);
        string lines[size];
        file = new Reader(file_name);
        for (int i=0; i<size; i++) {
            lines[i] = file->get_string();
        }
        delete(file);
        Rewriter *file_w = new Rewriter(file_name);
        int i, j;
        for (i=0; i<size; i++) {
            string trigger;
            for (j=0; j<lines[i].length(); j++) {
                trigger += lines[i][j];
            }
            if (lower_case(trigger)==comparator) {
                int end_num;
                end_num = 0;
                for (; i<size; i++) {
                    string new_line;
                    for (j=0; j<lines[i].length(); j++) {
                        if (lines[i][j]!=':') {
                            new_line += lines[i][j];
                        } else {
                            if (lower_case(new_line)=="day") {
                                new_line = new_line + ":  ";
                                new_line = new_line + to_string(obj.day);
                                end_num++;
                                break;
                            } else if (lower_case(new_line)=="night") {
                                new_line = new_line + ":  ";
                                new_line = new_line + to_string(obj.night);
                                end_num++;
                                break;
                            } else {
                                new_line += lines[i][j];
                            }
                        }
                    }
                    file_w->set_string_end(new_line);
                    if (end_num==2) {
                        break;
                    }
                }
            } else {
                file_w->set_string_end(lines[i]);
            }
        }
        delete(file_w);
    }
    public:
    Init_calcul_file(string name) {
        file_name = name;
    }
    InitDataList get_tariff() {
        InitDataList saved = data_searcher("tariffes");
        return saved;
    }
    InitDataList get_cheats() {
        InitDataList saved = data_searcher("cheats");
        return saved;
    }
    bool set_tariff(InitDataList obj) {
        data_changer("tariffes",obj);
        return true;
    }
    bool set_cheats(InitDataList obj) {
        data_changer("cheats",obj);
        return true;
    }
};
class Init_saved_file {
    string file_name;
    NameList names[size];
    int num;
    string lower_case(string str) {
        string out_data;
        for (int i=0; i<str.length(); i++) {
            out_data += tolower(str[i]);
        }
        return out_data;
    };
    public:
    Init_saved_file(string name) {
        file_name = name, num = 0;
        int size_line;
        size_line = 0;
        Reader *file;
        file = new Reader(file_name);
        while (file->is_end()) {
            file->get_string();
            size_line++;
        }
        delete(file);
        file = new Reader(file_name);
        int number_arr[size] = {};
        int serial_arr[size] = {};
        int n_num, s_num;
        n_num=0, s_num=0;
        for (int i=0; i<size_line; i++) {
            string line;
            line = file->get_string();
            string trigger1;
            for (int j=0; j<line.length(); j++) {
                if (line[j]!=';' && line.length()!=(j+1)) {
                    trigger1 += line[j];
                } else {
                    if (lower_case(trigger1)=="counter") {
                        for (++i; i<size_line; i++) {
                            string trigger2;
                            line = file->get_string();
                            for (j=0; j<line.length(); j++) {
                                if (line[j]!=';' && line.length()!=(j+1)) {
                                    trigger2 += line[j];
                                } else {
                                    if (lower_case(trigger2)=="number" || lower_case(trigger2)=="serial number") {
                                        string number;
                                        for (++j; j<line.length(); j++) {
                                            if (line[j]!=';' && line.length()!=(j+1)) {
                                                number += line[j];
                                            } else {
                                                if (line.length()==(j+1))
                                                    number += line[j];
                                                char new_number[number.length()+1];
                                                strcpy(new_number,number.c_str());
                                                if (lower_case(trigger2)=="number") {
                                                    try {
                                                        int num = atoi(new_number);
                                                        if (num)
                                                            throw (num);
                                                        else
                                                            throw (0);
                                                    } catch (int st) {
                                                        number_arr[n_num] = st;
                                                    }
                                                    n_num++;
                                                } else if (lower_case(trigger2)=="serial number") {
                                                    try {
                                                        int num = atoi(new_number);
                                                        if (num) {
                                                            throw (num);
                                                        } else
                                                            throw (0);
                                                    } catch (int st) {
                                                        serial_arr[s_num] = st;
                                                    }
                                                    s_num++;
                                                }
                                                number="";
                                                if (n_num>=size) {
                                                    n_num = 0;
                                                    break;
                                                } else if (s_num>=size) {
                                                    s_num = 0;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    } else {
                        break;
                    }
                }
            }
        }
        for (int i=0; i<size; i++) {
            names[i] = NameList(number_arr[i],serial_arr[i]);
        }
        delete(file);
    }
    bool add_new_tariff(string date,double day_t,double night_t) {
        int size_line;
        size_line = 0;
        Reader *file_r;
        file_r = new Reader(file_name);
        string line;
        while (file_r->is_end()) {
            file_r->get_string();
            size_line++;
        }
        delete(file_r);
        string lines[size_line];
        file_r = new Reader(file_name);
        for (int i=0; i<size_line; i++) {
            lines[i] = file_r->get_string();
        }
        delete(file_r);
        Rewriter *file_w;
        file_w = new Rewriter(file_name);
        int state[3] = {};
        for (int i=0; i<size_line; i++) {
            string trigger1;
            string pack;
            for (int j=0; j<lines[i].length(); j++) {
                if (lines[i][j]!=';' && lines[i].length()!=(j+1)) {
                    trigger1 += lines[i][j];
                } else {
                    if (lower_case(trigger1)=="date of changes" && !state[0]) {
                        pack = trigger1 + ";" + date;
                        for (; j<lines[i][j]; j++) {
                            pack += lines[i][j];
                        }
                        state[0] = 1;
                    } else if (lower_case(trigger1)=="day tariff" && !state[1]) {
                        pack = trigger1 + ";" + to_string(round(day_t*1000)/1000);
                        for (; j<lines[i][j]; j++) {
                            pack += lines[i][j];
                        }
                        state[1] = 1;
                    } else if (lower_case(trigger1)=="night tariff" && !state[2]) {
                        pack = trigger1 + ";" + to_string(round(night_t*1000)/1000);
                        for (; j<lines[i][j]; j++) {
                            pack += lines[i][j];
                        }
                        state[2] = 1;
                    } else {
                        pack = lines[i];
                    }
                    break;
                }
            }
            file_w->set_string_end(pack);
        }
        delete(file_w);
        return false;
    }
    NameList get_lich_info() {
        if (num<size) {
            NameList curr = names[num];
            num++;
            return curr;
        }
        return NameList(0,0);
    }
    bool is_exist() {
        if (num<size) {
            return true;
        }
        num = 0;
        return false;
    }
};
class Payment_history {
    string file_name;
    DataList data[size];
    string full_date;
    int num;
    bool search_doub(int arr[],int compar) {
        for (int i=0; i<size; i++) {
            if (arr[i]==compar)
                return true;
        }
        return false;
    }
    int ltwo_num(int num) {
        if (num>99) {
            int new_num = int(num/100);
            return num-(new_num*100);
        }
        return num;
    }
    public:
    Payment_history(string name) {
        file_name = name;
        num = 0;
    }
    bool add_payment(Lich_data *counters, double *checks, int ldate[], int cdate[]) {
        Endwriter *file_w;
        file_w = new Endwriter(file_name);
        string new_line1;
        new_line1 = "№/date";
        int names[size];
        int doubles[size];
        for (int i=0; i<size; i++) {
            names[i] = counters[i].get_name().name_number;
        }
        int i, j, n;
        for (i=0,n=0; i<size; i++) {
            int curr = names[i];
            for (j=i+1; j<size; j++) {
                if (curr==names[j]) {
                    if (doubles[n]!=curr) {
                        doubles[n] = curr;
                        n++;
                    }
                    names[j] = 0;
                }
            }
        }
        for (int i=0; i<size; i++) {
            new_line1 = new_line1 + ";" + to_string(counters[i].get_name().name_number);
            if (search_doub(doubles,counters[i].get_name().name_number)) {
                if (counters[i].get_name().serial_number!=0)
                    new_line1 = new_line1 + ":" + to_string(counters[i].get_name().serial_number);
                else
                    new_line1 = new_line1 + ":-";
            }
        }
        file_w->set_string_end(new_line1);
        string new_line2;
        new_line2 = to_string(ldate[0])+'.'+to_string(ldate[1])+'.'+to_string(ltwo_num(ldate[2]))+'-'+to_string(cdate[0])+'.'+to_string(cdate[1])+'.'+to_string(ltwo_num(cdate[2]));
        for (int i=0; i<size; i++) {
            new_line2 = new_line2 + ';' + to_string(checks[i]);
        }
        file_w->set_string_end(new_line2);
        delete(file_w);
        return true;
    }
    void set_date(string find_date) {
        for (int i=0; i<size; i++) {
            data[i] = DataList();
        }
        full_date = "";
        string costs[size];
        Reader *file_r;
        int size_line;
        file_r = new Reader(file_name);
        while (file_r->is_end()) {
            file_r->get_string();
            size_line++;
        }
        delete(file_r);
        file_r = new Reader(file_name);
        string last_line;
        string cfull_date;
        for (int i=0; i<size_line; i++) {
            string line = file_r->get_string();
            string date;
            int state;
            state = 0;
            for (int j=0; j<line.length(); j++) {
                if (line[j]!='-' && state==0) {
                    date += line[j];
                } else {
                    if (state==0) {
                        cfull_date = date;
                    }
                    state = 1;
                }
                if (line[j]!=';' && state==1) {
                    cfull_date += line[j];
                }
                if (line[j]==';') {
                    state = 2;
                    if (find_date==date) {
                        full_date = cfull_date;
                        int k;
                        k = 0;
                        string cost;
                        for (++j; j<line.length(); j++) {
                            if (line[j]!=';' && (j+1)<line.length()) {
                                cost += line[j];
                            } else {
                                if (!((j+1)<line.length())) {
                                    cost += line[j];
                                }
                                costs[k] = cost;
                                cost = "";
                                k++;
                            }
                        }
                        line = last_line;
                        for (j=0; j<line.length(); j++) {
                            if (line[j]==';')
                                break;
                        }
                        k=0;
                        string name, name1, name2;
                        for (++j; j<line.length(); j++) {
                            if (line[j]!=':' && line[j]!=';' && (j+1)<line.length()) {
                                name += line[j];
                            } else {
                                if (!((j+1)<line.length()) && line[j]!=':' && line[j]!=';') {
                                    name += line[j];
                                }
                                if (line[j]==':') {
                                    name1 = name;
                                    name = "";
                                } else {
                                    name2 = name;
                                    char arr_num[costs[k].length()+1];
                                    if (costs[k]=="") {
                                        data[k] = DataList(0,0,0);
                                        name = "", name1 = "", name2 = "";
                                        k++;
                                        continue;
                                    }
                                    strcpy(arr_num,costs[k].c_str());
                                    double number = atof(arr_num);
                                    if (name1!="" && name2!="") {
                                        char cname[name1.length()+1];
                                        char csname[name2.length()+1];
                                        strcpy(cname,name1.c_str());
                                        strcpy(csname,name2.c_str());
                                        int name = atoi(cname), sname = atoi(csname);
                                        data[k] = DataList(name,sname,number);
                                    } else if (name1=="" && name2!="") {
                                        char cname[name2.length()+1];
                                        strcpy(cname,name2.c_str());
                                        int name = atoi(cname);
                                        data[k] = DataList(name,0,number);
                                    } else {
                                        data[k] = DataList(0,0,number);
                                    }
                                    name = "", name1 = "", name2 = "";
                                    k++;
                                }
                            }
                        }
                    }
                    break;
                }
            }
            last_line = line;
        }
        delete(file_r);
    }
    DataList get_next_data() {
        if (num<size) {
            DataList obj = data[num];
            num++;
            return obj;
        }
        return DataList();
    }
    string get_date() {
        return full_date;
    }
    bool is_exist() {
        if (data[0].name_number==0 && data[1].name_number==0 && data[2].name_number==0) {
            return false;
        }
        if (num<size) {
            return true;
        }
        num = 0;
        return false;
    }
};
class Lichilniks_data_file {
    string file_name;
    DataList data[size];
    int num;
    public:
    Lichilniks_data_file(string name) {
        file_name = name;
        num = 0;
        ifstream file(file_name);
        if (file.peek()!=EOF) {
            update_data();
        }
    }
    bool initial_fdata(Init_saved_file *obj) {
        ifstream file(file_name);
        if (file.peek()==EOF) {
            Endwriter *file_w;
            file_w = new Endwriter(file_name);
            while (obj->is_exist()) {
                NameList name = obj->get_lich_info();
                file_w->set_string_end(to_string(name.serial_number)+":"+to_string(name.name_number)+";-");
            }
            delete(file_w);
            return true;
        }
        return false;
    }
    void update_data() {
        int size_line;
        size_line = 0;
        Reader *file_r;
        file_r = new Reader(file_name);
        while (file_r->is_end()) {
            file_r->get_string();
            size_line++;
        }
        delete(file_r);
        string lines[size_line];
        file_r = new Reader(file_name);
        for (int i=0; i<size_line; i++) {
            lines[i] = file_r->get_string();
        }
        delete(file_r);
        for (int i=0; i<size_line; i++) {
            string line = lines[i];
            for (int j=0; j<line.length(); j++) {
                string serial_num;
                for (serial_num=""; line[j]!=';' && j<line.length() && line[j]!=':'; j++) {
                    serial_num += line[j];
                }
                if (!((j+1)<line.length()))
                    break;
                j++;
                string name_num;
                for (name_num=""; line[j]!=';' && j<line.length(); j++) {
                    name_num+= line[j];
                }
                if (!((j+1)<line.length()))
                    break;
                j++;
                string value;
                for (value=""; line[j]!=';' && j<line.length(); j++) {
                    value+= line[j];
                }
                char cname[name_num.length()+1];
                char cserial[serial_num.length()+1];
                char cvalue[value.length()+1];
                strcpy(cname,name_num.c_str());
                strcpy(cserial,serial_num.c_str());
                strcpy(cvalue,value.c_str());
                int name, serial;
                double val;
                if (atoi(cname)) {
                    name = atoi(cname);
                    serial = atoi(cserial);
                    data[i] = DataList(name,serial,atof(cvalue));
                }
                break;
            }
        }
    }
    DataList search_data(NameList obj) {
        for (int i=0; i<size; i++) {
            if (data[i].name_number==obj.name_number&&data[i].serial_number==obj.serial_number) {
                return data[i];
            }
        }
        return DataList(obj.name_number,obj.serial_number,0);
    }
};


class Emulator {
    string file_name;
    public:
    Emulator(string name) {
        file_name = name;
    }
    void emulate_work() {
        Reader *file_r;
        file_r = new Reader(file_name);
        int line_size;
        line_size = 0;
        while (file_r->is_end()) {
            file_r->get_string();
            line_size++;
        }
        delete(file_r);
        string lines[line_size];
        file_r = new Reader(file_name);
        for (int i=0; i<line_size; i++) {
            lines[i] = file_r->get_string();
        }
        delete(file_r);
        Rewriter *file_w;
        file_w = new Rewriter(file_name);
        for (int i=0; i<line_size; i++) {
            string line = lines[i];
            string new_line;
            for (int j=0; j<line.length(); j++) {
                if (line[j]==';') {
                    string str_number;
                    for (++j; j<line.length(); j++) {
                        str_number += line[j];
                    }
                    char char_num[str_number.length()+1];
                    strcpy(char_num,str_number.c_str());
                    double number = atof(char_num);
                    number += double(rand()%100000)/1000;
                    new_line = new_line + ';' + to_string(number);
                    break;
                } else {
                    new_line += line[j];
                }
            }
            file_w->set_string_end(new_line);
        }
        delete(file_w);
    }
};
