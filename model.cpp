#include <iostream> 
#include <ctype.h>
#include <vector> 
#include <string>
#include <stack>
#include <algorithm>

using namespace std;

enum type_of_lex {
    LEX_NULL,                                                                                           /* 0*/
    LEX_AND, LEX_BREAK, LEX_BOOL, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_STRING, LEX_STRUCT,         /* 9*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_WHILE, LEX_WRITE, LEX_FOR, LEX_GOTO,          /*18*/
    LEX_FIN,                                                                                            /*19*/
    LEX_SEMICOLON, LEX_COMMA, LEX_DOT, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,  /*28*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_OPEN, LEX_CLOSE, /*38*/
    LEX_NUM,                                                                                            /*39*/
    LEX_ID,                                                                                             /*40*/
    LEX_STR,                                                                                            /*41*/
    TYPE_STRUCT,                                                                                        /*42*/
    POLIZ_LABEL,                                                                                        /*43*/
    POLIZ_ADDRESS,                                                                                      /*44*/
    POLIZ_GO,                                                                                           /*45*/
    POLIZ_FGO                                                                                           /*46*/
};

//////////////////////////////////////////////////////////////////////////////////////////////
class Ident{ 
    string  name;               //название переменной 
    bool declare;               //на этапе СА, встретилось ли это имя в разделе описаний
    type_of_lex type;           //тип лексемы 
    bool assign;                //в процессе интерпретации, когда переменная получит значение
    int value;                  //значение
    int num_in_STRUCT_TID;
    bool is_subobject;
    int num_of_object;
    string value_str;
public: 
    Ident():declare(false), assign(false){}

    Ident(string name){ 
        this->name = name;
        declare = false;
        assign = false;
    }
    
    bool get_is_subobject(){ 
        return is_subobject;
    }

    void set_is_subobject(){ 
        is_subobject = true;
    }

    void set_num_of_object(int num){ 
        num_of_object = num;
    }
    
    int get_num_of_object(){ 
        return num_of_object;
    }

    bool operator == (const string& s) const{ 
        return name == s;
    }
    
    int get_num_in_STRUCT_TID(){ 
        return num_in_STRUCT_TID;
    }

    void set_num_in_STRUCT_TID(int num){ 
        num_in_STRUCT_TID = num;
    }

    string get_name() const{ 
        return name;
    }

    void set_name(const string n){ 
        name = n;
    }

    bool get_declare() const{ 
        return declare;
    }

    void set_declare(){ 
        declare = true;
    }

    type_of_lex get_type()const{ 
        return type;
    }

    void set_type(type_of_lex t){
        type = t;
    }

    bool get_assign()const{
        return assign;
    }

    void set_assign(){ 
        assign = true;
    }

    int get_value() const{ 
        return value;
    }

    void set_value(int v){ 
        value = v;
    }

    void set_str_value(const string s){
        value_str = s;
    }

    string get_str_value()const{ 
        return value_str;
    }

};

//////////////////////////////////////////////////////////////////////////////////////////////
class Lex{ 
    type_of_lex t_lex;
    int         v_lex; 
public:
    Lex(type_of_lex t = LEX_NULL, int v = 0){ 
        t_lex = t;
        v_lex = v;
    }
    type_of_lex get_type()const{
        return t_lex;
    }
    int get_value()const{
        return v_lex;
    }
    void set_value(int v){ 
        v_lex = v;
    }

    friend ostream& operator << (ostream &S, Lex obj);
};

// таблица имен
vector <Ident> TID;                                                 //the table consist the identifaicator names. Each line == number of the IDENT apppearing
int put(const string &buf){ 
    vector <Ident> :: iterator k;
    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end()){ 
        cout << "MAN" << endl;
        return k - TID.begin();
    }
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}

vector <string> STR_TID;                                            //the table consist the STR_IDENT names. Each line == number of the STR appearing
int put_str(const string &buf){
    vector<string>::iterator k;
    if ((k = find(STR_TID.begin(), STR_TID.end(), buf)) != STR_TID.end()){ 
        return k - STR_TID.begin();
    }
    STR_TID.push_back(buf);
    return STR_TID.size() - 1;
}

struct strct{ 
    string name; 
    vector <Ident> TID_IN_STRUCT;
};

vector <strct> STRUCT_TID;




class Scanner{                                                      //scan program -> fix lexems -> give them info
    FILE *fp;
    char c;
    int look (const string buf, const char ** list){ 
        int i = 0;
        while (list[i]){ 
            if (buf == list[i]){
                return i;
            }
            i++;
        }
        return 0;
    }

    void gc(){ 
        c = fgetc(fp);
    }

public: 
    static const char *TW[], *TD[];
    Scanner(const char * program){ 
        if (!(fp = fopen (program, "r")))
            throw "can't open the file";
    }
    Lex get_lex();
};
const char * Scanner:: TW [] = {                      
                        "" , "and","break","bool","else","if","false", 
                        //0     1      2      3     4     5      6
                        "int", "string", "struct","not","or","program","read","true",
                        // 7     8         9        10   11      12      13    14 
                        "while","write", "for", "goto", NULL
                        // 15     16      17      18     19 
};
const char * Scanner:: TD [] = {
                        "@", ";", ",", ".", ":", "=", "(", ")", "==", 
                       //0    1    2    3    4    5    6    7    10  
                        "<",">","+","-","*","/","<=","!=",">=","{","}", NULL
                       //10 11   12  13 14  15   16   17   18   19, 20 , 21
};

int line = 1;
Lex Scanner::get_lex(){ 
    enum state {H, IDENT, NUMB, COM, ALE, NEQ, STR};
    int d, j;
    string buf;
    string str;
    state CS = H;

    do{ 
        gc();
        switch(CS){ 
            case H:
                if (c == ' ' || c == '\t' || c == '\n' || c == '\r'){
                    if (c == '\n'){ 
                        line++;
                    }
                }
                else if ( isalpha (c) ) {
                    buf.push_back (c);
                    CS = IDENT;
                } else if ( isdigit (c) ) {
                    d = c - '0';
                    CS = NUMB;
                }else if (c == '"'){ 
                    CS = STR;
                } else if ( c == '/' ) {
                    gc(); 
                    if (c == '*')
                        CS  = COM;
                    else{ 
                        ungetc(c, fp);
                        buf.push_back('/');
                        if ((j = look(buf, TD))){ 
                            return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                        } else{ 
                            throw c;
                        }
                    }
                } else if ( c == '=' || c == '<' || c == '>' ) { 
                    buf.push_back (c);
                    CS  = ALE; 
                } else if (c == '@')
                    return Lex (LEX_FIN );
                else if (c == '!') {
                    buf.push_back (c);
                    CS  = NEQ;
                } else if (c == '-'){ 
                    buf.push_back(c);
                    if ((j = look(buf, TD))){ 
                        return Lex ((type_of_lex)( j + (int)LEX_FIN), j);
                    }
                } else {
                    buf.push_back (c);
                    if ((j = look(buf, TD))){
                        return Lex ((type_of_lex)(j + (int)LEX_FIN), j);
                    } else
                        throw c;
                }
                break;
            case IDENT: 
                if ( isdigit(c) || isalpha(c) || c == '.'){ 
                    buf.push_back(c);
                } else{ 
                    ungetc(c, fp);
                    if (( j = look (buf, TW))){
                        return Lex((type_of_lex) (j), j);
                    } else { 
                        // cout << "new Ident     " << buf << endl; 
                        j = put(buf); 
                        return Lex(LEX_ID, j);
                    }
                }
                break;
            case NUMB: 
                if (isdigit(c)){ 
                    d = d * 10 + (c - '0'); 
                } else { 
                    ungetc(c, fp);
                    return Lex(LEX_NUM, d);
                } 
            break;
        case COM: 
            if (c == '*'){ 
                gc(); 
                if (c == '/'){ 
                    CS = H;
                } else { 
                    ungetc (c, fp);
                }
            } else if (c == '@'){ 
                throw c;
            }
            break;
        case ALE: 
            if (c == '='){ 
                buf.push_back (c);
                j = look (buf, TD);
                return Lex((type_of_lex)(j + (int)LEX_FIN), j);
            } else { 
                ungetc(c, fp);
                j  = look ( buf, TD );
                return Lex ((type_of_lex) ( j + (int) LEX_FIN), j);
            }
            break;
        case NEQ:
             if ( c == '=' ) {
                buf.push_back(c);
                j   = look (buf, TD);
                return Lex (LEX_NEQ, j);
            }
            else
                 throw '!';
            break;
        case STR: 
            while (c != '"'){ 
                str.push_back(c);
                gc();
            }
            //if (c == EOF) throw "string error";
            j = put_str(str);
            return Lex(LEX_STR, j);
            break;
        default:
            cout << "default" << c << endl;
        } //end switch
    } while(1);
}

ostream& operator << (ostream &str, Lex obj){ 
        string s; 
        if (obj.t_lex < LEX_FIN){ 
            s = Scanner::TW[obj.t_lex];
        }else if ( obj.t_lex >= LEX_FIN && obj.t_lex <= LEX_CLOSE){
            s = Scanner::TD[obj.t_lex - LEX_FIN ];
        }else if (obj.t_lex ==  LEX_ID){
            s = TID[obj.v_lex].get_name();
        } else if (obj.t_lex == LEX_STR) {
            s = STR_TID[obj.v_lex];
        } else if (obj.t_lex == LEX_NUM){
            s = "Numb";
        }else if (obj.t_lex == POLIZ_LABEL){
            s = "Label";
        }else if (obj.t_lex == POLIZ_ADDRESS){
            s = "Address";
        }else if (obj.t_lex == POLIZ_GO){
            s = "!";
        }else if (obj.t_lex == POLIZ_FGO){
            s = "!F";
        }else{
            throw obj;
        }
        str << '(' << s << ", " << obj.v_lex <<  ", " << type_of_lex(obj.t_lex)<< "):" << endl;
        return str;
    }

/////////////////////////////////////////// class Parser ////////////////////////////////////////////

template <class T, class T_EL>       
void from_st (T & st, T_EL & i ){
    i = st.top(); st.pop();
}
 /////////////////////////////////////////////////////
stack <vector <int> > break_stack;

vector <int> TGOTO;
vector <int> POLIZgoto;
vector <int> checkJump;

void check_jump(int n){ 
    vector <int>:: iterator k;
    if ((k = find (checkJump.begin(), checkJump.end(), n)) != checkJump.end()){ 
        cout << "twice the same label"; 
        throw "label"; 
    } else { 
        checkJump.push_back(n);
    }
}

int set_goto(const int buf, const int n){ 
    vector<int>::iterator k;
    if ((k = find(TGOTO.begin() , TGOTO.end(), buf)) != TGOTO.end()){ 
        return POLIZgoto[k - TGOTO.begin()];
    }
    if (n != 0){ 
        TGOTO.push_back(buf); 
        POLIZgoto.push_back(n);
    }
    return 0;
}

int check_str_id(const string str){ 
    cout << str << endl;
    cout << STRUCT_TID[0].name << endl;
    for (int i = 0; i < STRUCT_TID.size(); i++){ 
        if (str == STRUCT_TID[i].name){
            return 1;
        }
    }
    return 0;
}

class Parser{ 
    Lex          curr_lex;
    type_of_lex  c_type;
    int          c_val;
    Scanner      scan;
    stack < int >           st_int;
    stack < type_of_lex >   st_lex;                
    void  P();    
    void  ST();
    void  D1();
    void  D();
    void  B();
    void  S();
    void  E();
    void  OR();
    void  AND();
    void  E2();
    void  E1();
    void  T();
    void  F();
    void  dec ( type_of_lex type); 
    void  check_id_declared ();
    void  check_op ();
    void  check_not ();
    void  eq_type ();
    void  eq_bool ();
    int   check_id(string s);
    void  check_id_for_read ();
    void  gl (){
        curr_lex  = scan.get_lex();
        c_type    = curr_lex.get_type ();
        cout << curr_lex;                               //********
        c_val     = curr_lex.get_value ();
    }
public: 
    vector <Lex> poliz;         //внутреннее представление программ
    Parser ( const char *program ) : scan(program){ }
    void  analyze();            
};
void Parser::analyze () {
    gl ();
    P ();      
    // LEX_FIN                    
    if (c_type != LEX_FIN){
        cout << "error: " << line;
        throw curr_lex;
    }
    //for_each( poliz.begin(), poliz.end(), [](Lex l){ cout << l; });
    // вывод лексем из полиза
    cout << "\n\n\n\n" << endl;
    for ( Lex l : poliz ) 
        cout << l;
    cout << endl << "THE END OF ANALYZING!" << endl;
}

// начальное состояние
void Parser::P(){
    if ( c_type == LEX_PROGRAM ) {
        gl();
    } else {
        cout << "error: " << line;
        throw curr_lex; 
    }
    while (c_type == LEX_STRUCT){ 
        ST();
        gl();
    }
    D1 ();
    if ( c_type != LEX_OPEN){ 
        cout << "error: " << line;              
        throw curr_lex;
    }
    B ();
}

void Parser::ST(){ 
    type_of_lex buflex; 
    Ident bufIdent;
    strct buf;
    gl();
    if (c_type != LEX_ID){ 
        cout << "error: " << line << "struct name is needed" << endl; 
        throw curr_lex;
    }
    buf.name = TID[c_val].get_name();  // the name of the struct + c_val = the number in the TID;
    gl();
    if (c_type != LEX_OPEN){ 
         cout << "error: " << line << " '{' is needed in struct" << endl;
        throw curr_lex;
    }
    gl();
    while(c_type != LEX_CLOSE){ 
        if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL){ 
            if (c_type == LEX_STRING){ 
                buflex = LEX_STR;
            } else{ 
                buflex = c_type;
            }
        } else {
            cout << "error: " << line << " struct error" << endl;
            throw curr_lex;
        }
        gl();
        while (c_type != LEX_SEMICOLON){ 
            if (c_type != LEX_ID){  
                cout << "error: " << line << " struct error" << endl;
                throw curr_lex; 
            }
            bufIdent.set_name(TID[c_val].get_name());
            bufIdent.set_type(buflex);
            buf.TID_IN_STRUCT.push_back(bufIdent);
            gl();
            if (c_type == LEX_COMMA){ 
                gl();
            } else if (c_type != LEX_SEMICOLON) { 
                cout << "error: " << line << " ';'|',' is needed struct error" << endl;
                throw curr_lex;
            }
        }
        gl();
    }
    STRUCT_TID.push_back(buf);
}

int check_struct_name(const string & buf){ 
    //cout << "struct size == " << STRUCT_TID.size() << endl;
    //cout << STRUCT_TID[0].name << endl;
    for (int i = 0; i < STRUCT_TID.size(); i ++){
        if (STRUCT_TID[i].name == buf){ 
            return i;
        }
    }
    cout << "error: " << line << " not declacred type name" << endl;
    throw 'a';
}

// P -> int\sting\bool ID = [-]VALUE;
void Parser::D1(){ 
    if (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING){ 
        D ();                                   //проверяем, что следует за int bool string
        while (c_type == LEX_SEMICOLON){ 
            gl();
            if (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING){ 
                //cout << " line 428 " << endl;
                D ();
            } else break;
        }
    } else if (c_type == LEX_ID) { 
        int k = check_struct_name(TID[c_val].get_name());
        int buf_value;
        cout << "INFO" << endl;
        cout << " TID__c_val "<< c_val << "   name " << TID[c_val].get_name() << " ";
        
        gl ();
        int  buf_struct_val = c_val;
        string buf_name = TID[c_val].get_name();
        //buf_value = put(buf_name);
        st_int.push(c_val);
        TID[c_val].set_num_in_STRUCT_TID(k);
        dec(TYPE_STRUCT);
        cout << " NEEDED TID_c_val"<< c_val << "   name" << TID[c_val].get_name() << "    "<< TID[c_val].get_declare() << endl;
        string buf;
        
        for (int i = 0; i < STRUCT_TID[k].TID_IN_STRUCT.size(); i++){
            buf = buf_name + '.' + STRUCT_TID[k].TID_IN_STRUCT[i].get_name();
            cout << "new name of each IDENT from the struct " <<  buf << endl;
            buf_value = put(buf);               //VALUE FROM TID
            st_int.push(buf_value);
            dec(STRUCT_TID[k].TID_IN_STRUCT[i].get_type());
            TID[buf_value].set_is_subobject();
            TID[buf_value].set_num_of_object(buf_struct_val);
        }
// dec достает из стека все имена которые встретились и были положены в него 
// дальше проверяет встречалось ли оно раннее
// да ошибка 
// нет устанавливает declared и тип переменной
// ***** в dec устанваливаем тип перемнной
        gl();
        if (c_type != LEX_SEMICOLON){ 
            cout << "error: " << line << " cyntax error: ';' is needed" << endl; 
            throw curr_lex;
        }
        gl();
        if (c_type != LEX_OPEN){ 
            D1();
        }
    } else {
        cout << "error: " << line << endl;
        throw curr_lex;
    }
}

void Parser::D(){ 
    type_of_lex buf_type = c_type;                      // int bool str
 loop1:
    gl(); // type Identicator
    if (c_type != LEX_ID){
        cout << "error: " << line << " declaratoin error: name is needed"<< endl;
        throw curr_lex;
    }else{ 
        
        st_int.push(c_val);                        
        int buf_val = c_val;
        gl();
        if (c_type == LEX_ASSIGN){ 
            poliz.push_back(Lex(POLIZ_ADDRESS, buf_val));              // &a, _ , =    //a = ... положили a в ПОЛИЗ
            gl();
            if (( buf_type == LEX_BOOL && (c_type == LEX_TRUE || c_type == LEX_FALSE))||
                ( buf_type == LEX_INT && (c_type == LEX_NUM || c_type == LEX_MINUS))|| 
                ( buf_type == LEX_STRING && (c_type == LEX_STR || c_type == LEX_STRING))){ 
                if (c_type == LEX_MINUS){ 
                    gl();
                    if (c_type == LEX_NUM){ 
                        int tmp = curr_lex.get_value();
                        tmp = 0 - tmp;
                        curr_lex.set_value(tmp);
                    } else{ 
                        cout << "error: " << line << " number is needed " << endl;
                        throw curr_lex;
                    }
                }
                // poliz setter:
                poliz.push_back(curr_lex);                        // a = b положили в полиз b 
                poliz.push_back (Lex(LEX_ASSIGN));                // ПОЛИЗ &a,b,= 
                gl();
            }
            else { 
                cout << "error: " << line << " cannot initialize a variable with the value"<<  endl;
                throw curr_lex;
            }
        }
        if (c_type == LEX_COMMA){ 
            goto loop1;
        } 
        if (c_type != LEX_SEMICOLON){ 
            cout << "error: " << line;
            throw curr_lex;
        } else { 
            // buf_clear();
            if (buf_type == LEX_INT){ 
                dec(LEX_INT);
            } else if (buf_type == LEX_BOOL){ 
                dec(LEX_BOOL);
            } else if (buf_type == LEX_STRING){ 
                dec(LEX_STR);
            } else { 
                cout << "error: " << line;
                throw curr_lex;
            }
        }    
    }
}


void Parser::B(){
    if (c_type == LEX_OPEN) {
        gl ();
        S ();
        gl();
        while (c_type != LEX_CLOSE) {
            S ();
            gl();
        }
        if ( c_type == LEX_CLOSE ) {
            gl ();
        }
        else {
             cout << "error: " << line << " '}' is needed "; 
            throw curr_lex;
        }
    } else {
        cout << "error: " << line;
        throw curr_lex;
    }
}
    

// operators 
void Parser::S(){ 
    int pl0, pl1, pl2, pl3, pl4, pl5, pl6, pl7;
    
    if (c_type == LEX_IF){ 
        gl();
        if (c_type != LEX_LPAREN) throw curr_lex;
        gl();
        E();
        eq_bool();                          // проверили является выражение логическим
        pl2 = poliz.size();                 //if (...) {} =>  ... , '_', !F, 
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_FGO));
        if (c_type == LEX_RPAREN){ 
            gl();
            if (c_type == LEX_OPEN){ 
                gl();
            }
            S();                         
            pl3 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_GO));
            poliz[pl2] = Lex(POLIZ_LABEL, poliz.size());
            if (c_type == LEX_SEMICOLON){ 
                gl();
            }
            if (c_type == LEX_CLOSE){ 
                gl();
            }
            if (c_type == LEX_ELSE){ 
                gl();
                S();
                poliz[pl3] = Lex(POLIZ_LABEL, poliz.size());
            }
        } else throw curr_lex;
    } else if (c_type == LEX_OPEN){ 
        gl();
        S(); 
        gl();
        while (c_type != LEX_CLOSE){ 
            S();
            gl();
        }
    } else if (c_type == LEX_WHILE){ 
        break_stack.push(vector<int>());                       
        pl0 = poliz.size();                                     //for going back to the cycle beginning (in the POLIZ)
        gl();
        if(c_type != LEX_LPAREN){
            throw curr_lex;
        }
        gl();
        E ();
        eq_bool ();                                             //проверили является ли логическим выражнием 
        
        pl1 = poliz.size ();                                
        poliz.push_back (Lex());                                //to exit(quit) from while-cycle
        poliz.push_back (Lex(POLIZ_FGO)); 
        if(c_type == LEX_RPAREN){
            gl();
            S();

            poliz.push_back (Lex(POLIZ_LABEL, pl0));            //return to the beginning of the while
            poliz.push_back (Lex(POLIZ_GO));                    
            poliz[pl1] = Lex (POLIZ_LABEL, poliz.size());       //label to jump while the while-cycle is ended

                                                                //if we have 'break', we will exit the loop 
                                                                //end of the POLIZ = exit from the cycle where we will jump if we meet break
            while (!break_stack.top().empty()) { 
                poliz[break_stack.top().back()] = Lex(POLIZ_LABEL, poliz.size());  
                break_stack.top().pop_back();
            }
            break_stack.pop();
        } else{ 
            cout << "error: " << line;
            throw curr_lex;
        }
    }//end while
    else if( c_type == LEX_FOR ){
        break_stack.push(vector<int>());
        gl();
        if(c_type != LEX_LPAREN)                                        //for (
            throw curr_lex;
        gl();
        if(c_type != LEX_SEMICOLON){                                    //for (...
            E();
            if(c_type != LEX_SEMICOLON)                                 //for (... ; 
                throw curr_lex;
        }
        gl();
        pl6 = poliz.size();                                             //check 
        if(c_type != LEX_SEMICOLON){                                    //for ( ... ; ... 
            E();
            if(c_type != LEX_SEMICOLON)                                 //for ( ... ; ... ; 
                throw curr_lex;
        }
        gl();                                                       
        pl4 = poliz.size();                                             //to exit(quit) from while-cycle
        poliz.push_back(Lex());                                 
        poliz.push_back (Lex(POLIZ_FGO));

        pl5 = poliz.size();                                             //to jump to the beginnig of the for-cycle
        poliz.push_back(Lex());
        poliz.push_back (Lex(POLIZ_GO));

        pl7 = poliz.size();                                              //label to change the conditions (3-rd section)
        if(c_type != LEX_SEMICOLON && c_type != LEX_RPAREN){             //for ( ... ; ... ; ...
            E();
            if(c_type != LEX_RPAREN){                                    //for ( ... ; ... ; ... )
                cout << "error :" << line << " '{' is needed "; 
                throw curr_lex;
            }
        }

        poliz.push_back(Lex(POLIZ_LABEL, pl6));                         //jump to check the conditions
        poliz.push_back(Lex(POLIZ_GO));
        poliz[pl5] = Lex(POLIZ_LABEL, poliz.size());                    //the beginning of the for-cycle 
        cout << "not errror " << curr_lex; 
        gl();
        S();
        poliz.push_back(Lex(POLIZ_LABEL, pl7));                         //jump to change 3-rd section
        poliz.push_back (Lex(POLIZ_GO));

        poliz[pl4] = Lex(POLIZ_LABEL, poliz.size());                    //the exit of the for-cycle 
        while (!break_stack.top().empty()){ 
            poliz[break_stack.top().back()] = Lex(POLIZ_LABEL, poliz.size());
            break_stack.top().pop_back();
        }
        break_stack.pop();
    } else if( c_type == LEX_BREAK){
        if(break_stack.size() == 0){
            cout << "error: " << line << " break isnt worked";
            throw curr_lex;
        }

        break_stack.top().push_back(poliz.size());
        poliz.push_back(Lex());                                         //jump
        poliz.push_back (Lex (POLIZ_GO));
        gl();
        if(c_type != LEX_SEMICOLON){
            cout << "error: " << line<< " semicolon after BREAK needed";
            throw curr_lex;
        }
    }//end break
    else if (c_type == LEX_READ) {
        gl ();
        if (c_type == LEX_LPAREN) {
            gl ();
            if (c_type == LEX_ID) {
                check_id_for_read();                                    // check if it is declared
                poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
            } else {
                cout << "error: " << line; 
                throw curr_lex;
            }
            gl();
            if (c_type == LEX_RPAREN) {
                gl ();
                if(c_type != LEX_SEMICOLON) {
                    throw curr_lex;
                }
                poliz.push_back (Lex(LEX_READ));    
            } else{ 
                cout << "error: " << line; 
                throw curr_lex;
            }
        } else{
            cout << "error: " << line; 
            throw curr_lex;
        }
    }//end read
    else if (c_type == LEX_WRITE) {
        cout << "LEX_WRITE" << endl;
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            E ();
            poliz.push_back (Lex(LEX_WRITE));
            while(c_type == LEX_COMMA){
                gl();
                E();
                poliz.push_back (Lex(LEX_WRITE));
            }
            if (c_type == LEX_RPAREN){
                gl();
                if(c_type != LEX_SEMICOLON) {
                    cout << "error: " << line <<" semicolon needed";
                    throw curr_lex;
                }
            }
            else { 
                cout << "error: " << line;
                throw curr_lex;
            }
        }
        else { 
            cout << "error: " << line;
            throw curr_lex;
        }
    }//end write
    else if(c_type == LEX_GOTO){
        gl();
        if (c_type == LEX_ID) {
            int buf = set_goto(c_val, 0);                                               // проверили встречалось ли оно ранее
            if (buf != 0) {                                                             // да 
                poliz.push_back (Lex(POLIZ_LABEL , buf));                               
                poliz.push_back(Lex(POLIZ_GO));
            } else{                                                                     // нет!
                set_goto(c_val, poliz.size());                                          // (goto N) сохраниили новый N 
                                                                                        // а именно в таблице TGOTO лежит значение ident
                                                                                        // в таблице NUMJ лежит тек положение полиза для того, 
                                                                                        // чтобы в дальшейщем положить в полиз метку перехода
                poliz.push_back(Lex());                                                 
                poliz.push_back(Lex(POLIZ_GO));
            }
            gl();
            if (c_type != LEX_SEMICOLON){ 
                cout << "error: " << line << " ';' is needed"; 
                throw curr_lex;
            }
        } else { 
            cout << "error: " << line << " goto ";
            throw curr_lex;
        }
    } else if (c_type == LEX_ID) { 
        if (TID[c_val].get_declare() == false){
            int buf = c_val;
            gl();
            if(c_type == LEX_COLON){            
                check_jump(buf);                                                       //есть ли еще одна метка?
                int buf1; 
                if ((buf1 = set_goto(buf, poliz.size())) != 0 ){                       //если был goto => 
                                                                                        // в buf8 номер-метка в полизе (goto N;) 
                    poliz[buf1] = Lex(POLIZ_LABEL, poliz.size());                       // положили место в полизе куда надо перейти по метке goto N;
                }
                gl();
                S();
            } else { 
                cout << "error: " << line << " ID is not declared" ; 
                throw curr_lex;
            }
        } else {
            E();
        }
    }
}


void Parser:: E(){ 
    OR();
    if (c_type == LEX_ASSIGN){ 
        cout << "E";
        gl(); 
        E();
        eq_type();                                                              //проверили совпадение типов ___ = ___ 
        poliz.push_back(Lex(LEX_ASSIGN));
    }
}

void Parser::OR() { 
    AND();
    if (c_type == LEX_OR){ 
        cout << "OR" << endl;
        st_lex.push(c_type);
        gl();
        AND();
        check_op();
    }
}

void Parser::AND(){ 
    E2();
    if (c_type == LEX_AND){
        cout << "AND" << endl;
        st_lex.push(c_type); 
        gl();
        E2();
        check_op();
    }
}

void Parser::E2(){
    E1();
    if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR || 
        c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ){ 
        st_lex.push(c_type);
        cout << "E2" << endl;
        gl();
        E1();
        check_op();
    }
}

// с помощью check_op() достаем из стека лексемы и операцию 
// проверяем совпадение типов 
// после кладем лексему в стек lex_int lex_int => lex_int
// в конце кладем в полиз операцию

void Parser::E1 () {
    T ();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS ) {
        cout << "E1" << endl;
        st_lex.push(c_type);
        gl ();
        T ();
        check_op ();
    }
}

void Parser::T(){ 
    F();
    while(c_type == LEX_TIMES || c_type == LEX_SLASH){ 
        cout << "T" << endl;
        st_lex.push(c_type);
        gl();
        F();
        check_op();
    }
}


void Parser::F(){ 
    if (c_type == LEX_ID){
        check_id_declared();                                            //проверить лексему и положить в стек лексему(тип)
        int buf_val = c_val;
        int buf_type = c_type;
        gl();
        if(c_type == LEX_ASSIGN){ 
            /*if (buf_type == TYPE_STRUCT){ 
                cout << "DINNNN" << endl;
                poliz.push_back(Lex(POLI, buf_val));
            }else { */
                poliz.push_back(Lex(POLIZ_ADDRESS, buf_val));
        } else {
            poliz.push_back(Lex(LEX_ID, buf_val));                 
        }
    } else if (c_type == LEX_NUM){                              
        st_lex.push(LEX_INT);
        poliz.push_back(curr_lex);
        gl();
    } else if (c_type == LEX_STR){                             
        st_lex.push(LEX_STR);
        poliz.push_back(curr_lex);
        gl();
    } else if (c_type == LEX_TRUE){                             
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex (LEX_TRUE, 1));
    } else if (c_type == LEX_FALSE){                           
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(LEX_FALSE, 0));
        gl();
    } else if (c_type == LEX_NOT){                             
        gl();
        F();
        check_not();                                                     // проверили является след лексема логическая
    } else if (c_type == LEX_LPAREN){                            
        gl();
        E();
        if (c_type == LEX_RPAREN){
            gl();                         
        } else {
            throw curr_lex;
        } 
    } else if (c_type == LEX_MINUS) { 
        Lex null(LEX_NUM, 0);
        cout << "null" << endl;
        st_lex.push(LEX_INT);
        poliz.push_back(null);
    } else  {
        cout << "error: " << line;
        throw curr_lex;
    }
}
///////////////////////////////////////////////////////////////////////////
                                            //(если встретились IDENT помемщаем их в стек) =>
void Parser::dec (type_of_lex type) {       //обрабатываем int a, b, c ,d, a => устанавливаем флаги declare
    int i;
    while (!st_int.empty()){
        from_st (st_int, i);
        if ( TID[i].get_declare()) {
            cout << "error: " << line;
            throw " twice declared";
        } else {
            TID[i].set_declare();
            TID[i].set_type(type);
        }
    }
}


void Parser::check_op(){
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;

    from_st(st_lex, t2);
    from_st(st_lex, op);
    from_st(st_lex, t1);
    if (t1 == t2 && (t1 == LEX_STR || t1 == LEX_STRING)){ 
        // строки
        if (op == LEX_LSS || op == LEX_GTR || op == LEX_EQ || op == LEX_NEQ){ 
            st_lex.push(LEX_BOOL);
        } else if (op == LEX_PLUS  || op == LEX_EQ){ 
            st_lex.push(LEX_STR);
        } else{ 
            cout << "error : " << line;
            throw " incorrect operator is in the opearation";
        }
    } else { 
        if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH){ 
            r = LEX_INT;
        }
        if (op == LEX_OR || op == LEX_AND){ 
            t = LEX_BOOL;
        }
        if (t1 == t2 && t1 == t){ 
            st_lex.push(r);
        } else { 
            cout << "error2 : " << line;
            throw " incorrect types are in the operation";
        }
    }
    //необходимо положить в ПОЛИЗ операцию ..., t1, t2, op, ... 
    poliz.push_back(Lex(op)); 
}

void Parser:: check_not(){ 
    if (st_lex.top() != LEX_BOOL){ 
        throw "wrong type is in not";
    } else{ 
        poliz.push_back(Lex(LEX_NOT));
    }
}

void Parser::eq_type(){ 
    type_of_lex type;
    from_st(st_lex, type);
    //cout << type << st_lex.top() << endl;
    if (type != st_lex.top()){ 
        cout << "error: " << line ;
        throw "types are different in '='";
    }
}

void Parser::eq_bool(){ 
    if (st_lex.top() != LEX_BOOL)
        throw "expression is not boolean";
    st_lex.pop();
}

void Parser::check_id_for_read(){ 
    if (!TID[c_val].get_declare())
        throw "not declared";
}

void Parser::check_id_declared(){ 
    if (TID[c_val].get_declare()){ 
        cout << TID[c_val].get_type() << endl;
        st_lex.push(TID[c_val].get_type());
    } else { 
        throw "not decalred";
    }
}

int Parser::check_id(string s){ 
    for (int i = 0; i < TID.size() ; i ++){ 
        if (s == TID[i].get_name()){ 
            return 1;
        }
    }
    return 0;
}




//////////////////////////////////////////////////////////////////
//пробегаемся по полизу
//


int check_1(string buf){ 
    for (int i = 0; i < STRUCT_TID.size(); i++){ 
    
        if ( buf == STRUCT_TID[i].name){ 
            return STRUCT_TID[i].TID_IN_STRUCT.size();
        }
    }
    return 0;
}


bool check_declare_obj(int val){
    int buf_val = TID[val].get_num_of_object();
    int buf3 = TID[buf_val].get_num_in_STRUCT_TID();
    int size = STRUCT_TID[buf3].TID_IN_STRUCT.size();
    for (int i = 1; i <= size; i ++){ 
        if (!TID[buf_val + i].get_assign()){
            return false;
        }
    }
    return true;
}

void eq_type_struct(int val1, int val2){ 
    if ( TID[val1].get_num_in_STRUCT_TID() != TID[val2].get_num_in_STRUCT_TID()){ 
        throw "error: incorrect types in the object equalation";
    }
}

class Executer{ 
public:
    void execute(vector <Lex> & poliz){ 
        Lex pc_el;
        stack <int> args;
        stack <type_of_lex> argt;
        int i, j, index = 0, size = poliz.size();
        int buf_val3, buf333;
        string x, y; 
        string buf ;
        int buf3, buf2, buf_size;
        type_of_lex bufLex;
        while (index < size){ 
            pc_el = poliz[index]; 
            //cout << ((type_of_lex)(pc_el.get_type())) ;
           //  cout << "val" << pc_el.get_value()<<endl;
            switch (pc_el.get_type()){ 
            
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case LEX_STR: case POLIZ_ADDRESS: case POLIZ_LABEL:
                args.push(pc_el.get_value()); 
                argt.push(pc_el.get_type());
                break; 

            case LEX_ASSIGN: 
                from_st ( args, i );
                from_st ( args, j );
                type_of_lex bufLex1;
                from_st (argt, bufLex1);
                from_st (argt, bufLex);

                //cout << "TRY " << i << endl;
                if (TID[j].get_type() == TYPE_STRUCT){ 
                    eq_type_struct(i, j);
                    buf2 = TID[j].get_num_in_STRUCT_TID();
                    buf_size = STRUCT_TID[buf2].TID_IN_STRUCT.size();
                    TID[j].set_assign();
                    for (int s = 1; s <= buf_size; s++){ 
                        //cout << TID[ i + s].get_value() ;
                        //cout << " " << TID[i + s].get_name() << endl;
                        TID[j + s].set_value(TID[i + s].get_value()); 
                        TID[j + s].set_assign();
                    }
                    args.push(i);
                    argt.push(bufLex1);
                } else { 
                    TID[j].set_value(i); 
                    TID[j].set_assign();
                    args.push(i);
                    argt.push(bufLex1);
                    buf_val3 = j;
                    //cout << TID[buf_val3].get_name() << endl;
                    //cout << TID[buf_val3].get_is_subobject() << endl;
                    if (TID[buf_val3].get_is_subobject()){
                        if (check_declare_obj(buf_val3) == true){ 
                            buf333 = TID[buf_val3].get_num_of_object();
                            TID[buf333].set_assign();
                        }
                    }
                }
                break;

            case LEX_ID:
                //cout << "LINE" << line  << " " << TID[pc_el.get_value()].get_name() << endl;
                i = pc_el.get_value();
                if (TID[i].get_assign()){
                    if (TID[i].get_type() == TYPE_STRUCT){ 
                        args.push(i);
                        argt.push(TID[i].get_type());
                        /*buf_size = STRUCT_TID[TID[i].get_num_in_STRUCT_TID()].TID_IN_STRUCT.size();
                        for (int index = 1; index < size; index ++){ 
                            cout << TID[ i + index].get_value() << endl;
                            cout << " " << TID[i+index].get_name() << endl;
                            args.push(TID[i + index].get_value()); 
                            argt.push(TID[i + index].get_type());
                        }*/
                    } else {
                        args.push(TID[i].get_value()); 
                        argt.push(TID[i].get_type());
                    }
                    break;
                } else{ 
                    cout << TID[i].get_name();
                    throw "POLIZ: indefinite identifier";
                }
            
            case LEX_NOT: 
                from_st(args, i);
                args.push(!i); 
                break;

            case LEX_OR: 
                from_st (args, i); 
                from_st (args, j);
                args.push ( j || i );
                from_st(argt, bufLex);
                break;

            case LEX_AND: 
                from_st (args, i); 
                from_st (args, j);
                args.push ( j && i );
                from_st(argt, bufLex);
                break;

            case POLIZ_GO:
                from_st (args, i);
                index = i - 1;
                from_st ( argt, bufLex);
                break;
 
            case POLIZ_FGO:
                from_st (args, i);
                from_st (args, j);
                if ( !j ) index = i - 1;
                from_st (argt, bufLex);
                from_st (argt, bufLex);
                break;

             case LEX_WRITE:
                from_st (args, j);
                from_st (argt, bufLex);
                if( bufLex == LEX_STRING || bufLex == LEX_STR) {
                    cout << STR_TID[j]<<endl;
                } else {
                    cout << j << endl;
                }
                break; 
 
            case LEX_READ:
                int k;
                from_st (args, i);
                from_st (argt, bufLex);
                if ( TID[i].get_type () == LEX_INT ) {
                    cout << "Input int value for" << TID[i].get_name () << endl;
                    cin >> k;
                } else if (TID[i].get_type() == LEX_BOOL){
                    string j;
                    while (1) {
                        cout << "Input boolean value (true or false) for " << TID[i].get_name() << endl;
                        cin >> j;
                        if ( j != "true" && j != "false" ) {
                            cout << "Error in input:true/false" << endl;
                            continue;
                        }
                        k = ( j == "true" ) ? 1 : 0;
                        break;
                    }
                } else {
                    cout << " Input string value for " << TID[i].get_name() << endl;
                    string bufstr;
                    cin >> bufstr; 
                    k = put_str(bufstr);
                }
                TID[i].set_value(k);
                TID[i].set_assign();
                break;

            case LEX_PLUS:
                from_st ( args, i );
                from_st ( args, j );
                from_st(argt, bufLex); 
                if (bufLex == LEX_STR || bufLex == LEX_STRING){ 
                    string bufstr = STR_TID[j] + STR_TID[i]; 
                    int k = put_str(bufstr);
                    args.push(k);
                } else { 
                    args.push (i + j); 
                }
                break;

            case LEX_MINUS:
                from_st ( args, i );
                from_st ( args, j );
                from_st(argt, bufLex); 
                args.push (j - i); 
                break; 
            
            case LEX_TIMES: 
                from_st ( args, i );
                from_st ( args, j );
                from_st(argt, bufLex); 
                args.push (i * j); 
                break; 

            case LEX_SLASH:
                from_st ( args, i );
                from_st ( args, j );
                from_st (argt, bufLex);
                if (i){
                    args.push ( j / i );
                    break;
                } else{
                    cout << "error: " << line ;
                    throw " POLIZ: divide by zero";
                }

            case LEX_EQ: 
                from_st ( args, i );
                from_st ( args, j );
                from_st (argt, bufLex);
                from_st (argt, bufLex);
                if(bufLex == LEX_STR || bufLex == LEX_STRING){
                    args.push(STR_TID[j] == STR_TID[i]);
                } else args.push(i == j);
                argt.push(LEX_BOOL);
                break;

            case LEX_NEQ: 
                from_st ( args, i );
                from_st ( args, j );
                from_st (argt, bufLex);
                from_st (argt, bufLex);
                if(bufLex == LEX_STR || bufLex == LEX_STRING){
                    args.push(STR_TID[j] != STR_TID[i]);
                } else args.push(i != j);
                argt.push(LEX_BOOL);
                break;

            case LEX_LSS: 
                from_st ( args, i );
                from_st ( args, j );
                from_st (argt, bufLex);
                from_st (argt, bufLex);

                if(bufLex == LEX_STR || bufLex == LEX_STRING){
                    args.push(STR_TID[j] < STR_TID[i]);
                } else 
                    args.push(j < i);
                argt.push(LEX_BOOL);
                break;

            case LEX_GTR:
                from_st ( args, i );
                from_st ( args, j );
                from_st (argt, bufLex);
                from_st (argt, bufLex);

                if(bufLex == LEX_STR || bufLex == LEX_STRING){
                    args.push(STR_TID[j] > STR_TID[i]);
                } else 
                    args.push(j > i);
                argt.push(LEX_BOOL);
                break;

            case LEX_LEQ: 
                from_st ( args, i );
                from_st ( args, j );
                from_st (argt, bufLex);
                from_st (argt, bufLex);

                if(bufLex == LEX_STR || bufLex == LEX_STRING){
                    args.push(STR_TID[j] <= STR_TID[i]);
                } else 
                    args.push(j <= i);
                argt.push(LEX_BOOL);
                break;

            case LEX_GEQ: 
                from_st ( args, i );
                from_st ( args, j );
                from_st (argt, bufLex);
                from_st (argt, bufLex);

                if(bufLex == LEX_STR || bufLex == LEX_STRING){
                    args.push(STR_TID[j] >= STR_TID[i]);
                } else 
                    args.push(j >= i);
                argt.push(LEX_BOOL);
                break;

            default: 
                break;
            }  //end of switch
            ++index;
        }//end of while
        cout << "Finish of executing!!!" << endl;
    }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Interpretator { 
    Parser parserator;
    Executer executator;
public: 
    Interpretator (const char * program): parserator(program){} 
    void interpretation() { 
        parserator.analyze();
        cout << "execute" <<endl;

        executator.execute(parserator.poliz);
    }
};

////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv){
    try{ 
        if (argc != 2) throw 1;
        /*Scanner scan (argv[1]);
        Lex l1 = scan.get_lex();
        while (l1.get_type() != LEX_FIN){ 
            cout << l1;
            l1 = scan.get_lex();
        }

        Parser pars(argv[1]);
        pars.analyze();
        */
        Interpretator I(argv[1]); 
        I.interpretation();
        return 0;
    }
    catch (int){ 
        cout << "not enougth arguments in the command line" << endl;
        return 1;
    }
    catch (char c){ 
        cout << "unexpected symbol" << c << endl; 
        return 1;
    }
    catch (Lex l){ 
        cout << "lexeme error" << l << endl;
        return 1;
    }
    catch (const char * str){ 
        cout << str << endl;
        return 1;
    } 
}