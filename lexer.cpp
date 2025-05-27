#include<iostream>
#include<string>
#include<vector>
#include<cctype>
#include<fstream>
using namespace std;

enum TokenType {KEYWORD, IDENTIFIER, NUMBER, OPERATOR, SYMBOL, COMMENT, WHITESPACE, PREPROCESSOR, STRING, ERROR, END};

struct Token{
    TokenType type;
    string value;
    string name;
};

class Lexer{
    string input;
    size_t pos;
    string mode;
    vector<string> keywords={"alignas","alignof","and","and_eq","asm","auto","bitand","bitor","bool","break","case","catch","char","char8_t","char16_t","char32_t","class","compl","concept","const","consteval","constexpr","constinit","const_cast","continue","co_await","co_return","co_yield","decltype","default","delete","do","double","dynamic_cast","else","enum","explicit","export","extern","false","float","for","friend","goto","if","inline","int","long","mutable","namespace","new","noexcept","not","not_eq","nullptr","operator","or","or_eq","private","protected","public","register","reinterpret_cast","requires","return","short","signed","sizeof","static","static_assert","static_cast","struct","switch","template","this","thread_local","throw","true","try","typedef","typeid","typename","union","unsigned","using","virtual","void","volatile","wchar_t","while","xor","xor_eq"};
    vector<string> preprocessor_directives={"include","define","ifdef","ifndef","endif","else","elif","undef","pragma"};

    bool isKeyword(string str){
        for(auto kw:keywords){
            if(kw==str) return true;
        }
        return false;
    }

    bool isPreprocessorDirective(string str){
        for(auto dir:preprocessor_directives){
            if(dir==str) return true;
        }
        return false;
    }

    string getTokenName(TokenType type,string value){
        switch(type){
            case KEYWORD:return "keyword";
            case IDENTIFIER:return "identifier";
            case NUMBER:return "integer";
            case OPERATOR:
                if(value=="+") return "addition";
                if(value=="-") return "subtraction";
                if(value=="*") return "multiplication";
                if(value=="/") return "division";
                if(value=="=") return "assignment";
                if(value=="==") return "equal";
                if(value=="!=") return "not equal";
                if(value=="<=") return "less than or equal";
                if(value==">=") return "greater than or equal";
                if(value=="&&") return "logical and";
                if(value=="||") return "logical or";
                if(value=="++") return "increment";
                if(value=="--") return "decrement";
                if(value=="<<") return "left shift/insertion";
                if(value==">>") return "right shift/extraction";
                if(value=="+=") return "addition assignment";
                if(value=="-=") return "subtraction assignment";
                if(value=="*=") return "multiplication assignment";
                if(value=="/=") return "division assignment";
                if(value=="%=") return "modulus assignment";
                if(value=="&=") return "bitwise and assignment";
                if(value=="|=") return "bitwise or assignment";
                if(value=="^=") return "bitwise xor assignment";
                if(value=="<<=") return "left shift assignment";
                if(value==">>=") return "right shift assignment";
                if(value=="->") return "member access";
                if(value=="&") return "bitwise and/address";
                if(value=="|") return "bitwise or";
                if(value=="^") return "bitwise xor";
                if(value=="%") return "modulus";
                if(value=="!") return "logical not";
                if(value=="~") return "bitwise not";
                if(value=="<") return "less than";
                if(value==">") return "greater than";
                return "operator";
            case SYMBOL:
                if(value==";") return "semicolon";
                if(value=="(") return "left parenthesis";
                if(value==")") return "right parenthesis";
                if(value=="<") return "less than";
                if(value==">") return "greater than";
                if(value=="{") return "left brace";
                if(value=="}") return "right brace";
                if(value=="[") return "left bracket";
                if(value=="]") return "right bracket";
                if(value==",") return "comma";
                if(value==".") return "dot";
                if(value==":") return "colon";
                if(value=="::") return "scope resolution";
                if(value=="?") return "conditional";
                if(value=="@") return "at symbol";
                return "symbol";
            case COMMENT:
                if(value.substr(0,2)=="//") return "single-line comment";
                return "multi-line comment";
            case WHITESPACE:
                if(value==" ") return "space";
                if(value=="\t") return "tab";
                if(value=="\n") return "newline";
                return "whitespace";
            case PREPROCESSOR:
                if(value=="#") return "preprocessor symbol";
                if(value=="include") return "include directive";
                if(value=="define") return "define directive";
                if(value=="ifdef") return "ifdef directive";
                if(value=="ifndef") return "ifndef directive";
                if(value=="endif") return "endif directive";
                if(value=="else") return "else directive";
                if(value=="elif") return "elif directive";
                if(value=="undef") return "undef directive";
                if(value=="pragma") return "pragma directive";
                return "preprocessor directive";
            case STRING:return "string literal";
            case ERROR:return "invalid character";
            case END:return "end of input";
            default:return "unknown";
        }
    }

public:
    Lexer(string src,string m):input(src),pos(0),mode(m){}

    Token nextToken(){
        if(mode=="compiler"){
            while(pos<input.length()&&isspace(input[pos])){
                pos++;
            }
        }
        if(mode=="general"&&pos<input.length()&&isspace(input[pos])){
            string ws;
            while(pos<input.length()&&isspace(input[pos])){
                ws+=input[pos++];
                if(ws==" "||ws=="\t"||ws=="\n") break;
            }
            return{WHITESPACE,ws,getTokenName(WHITESPACE,ws)};
        }
        if(pos>=input.length()){
            return{END,"",mode=="general"?getTokenName(END,""):""};
        }
        char current=input[pos];
        if(current=='#'){
            pos++;
            return{PREPROCESSOR,"#",mode=="general"?getTokenName(PREPROCESSOR,"#"):""};
        }
        if(current=='/'){
            if(pos+1<input.length()){
                if(input[pos+1]=='/'){
                    string comment="//";
                    pos+=2;
                    while(pos<input.length()&&input[pos]!='\n'){
                        comment+=input[pos++];
                    }
                    if(mode=="general"){
                        return{COMMENT,comment,getTokenName(COMMENT,comment)};
                    }
                    return nextToken();
                }
                if(input[pos+1]=='*'){
                    string comment="/*";
                    pos+=2;
                    while(pos+1<input.length()&&!(input[pos]=='*'&&input[pos+1]=='/')){
                        comment+=input[pos++];
                    }
                    if(pos+1<input.length()){
                        comment+="*/";
                        pos+=2;
                    }else{
                        return{ERROR,comment,mode=="general"?getTokenName(ERROR,comment):""};
                    }
                    if(mode=="general"){
                        return{COMMENT,comment,getTokenName(COMMENT,comment)};
                    }
                    return nextToken();
                }
            }
        }
        if(current=='"'){
            string str="\"";
            pos++;
            while(pos<input.length()&&input[pos]!='"'){
                str+=input[pos++];
            }
            if(pos<input.length()&&input[pos]=='"'){
                str+="\"";
                pos++;
                return{STRING,str,mode=="general"?getTokenName(STRING,str):""};
            }else{
                return{ERROR,str,mode=="general"?getTokenName(ERROR,str):""};
            }
        }
        if(isalpha(current)||current=='_'){
            string id;
            while(pos<input.length()&&(isalnum(input[pos])||input[pos]=='_')){
                id+=input[pos++];
            }
            if(isKeyword(id)){
                return{KEYWORD,id,mode=="general"?getTokenName(KEYWORD,id):""};
            }
            if(isPreprocessorDirective(id)){
                return{PREPROCESSOR,id,mode=="general"?getTokenName(PREPROCESSOR,id):""};
            }
            return{IDENTIFIER,id,mode=="general"?getTokenName(IDENTIFIER,id):""};
        }
        if(isdigit(current)){
            string num;
            while(pos<input.length()&&isdigit(input[pos])){
                num+=input[pos++];
            }
            return{NUMBER,num,mode=="general"?getTokenName(NUMBER,num):""};
        }
        if(current=='+'||current=='-'||current=='*'||current=='/'||current=='='||
           current=='&'||current=='|'||current=='^'||current=='%'||current=='!'||
           current=='~'||current=='<'||current=='>'){
            string op=string(1,current);
            pos++;
            if(pos<input.length()){
                char next=input[pos];
                if(pos+1<input.length()&&
                   ((current=='<'&&next=='<'&&input[pos+1]=='=')||
                    (current=='>'&&next=='>'&&input[pos+1]=='='))){
                    op+=next;
                    op+=input[pos+1];
                    pos+=2;
                }else if((current=='='&&next=='=')||
                         (current=='!'&&next=='=')||
                         (current=='<'&&next=='=')||
                         (current=='>'&&next=='=')||
                         (current=='&'&&next=='&')||
                         (current=='|'&&next=='|')||
                         (current=='+'&&next=='+')||
                         (current=='-'&&next=='-')||
                         (current=='<'&&next=='<')||
                         (current=='>'&&next=='>')||
                         (current=='+'&&next=='=')||
                         (current=='-'&&next=='=')||
                         (current=='*'&&next=='=')||
                         (current=='/'&&next=='=')||
                         (current=='%'&&next=='=')||
                         (current=='&'&&next=='=')||
                         (current=='|'&&next=='=')||
                         (current=='^'&&next=='=')){
                    op+=next;
                    pos++;
                }
            }
            return{OPERATOR,op,mode=="general"?getTokenName(OPERATOR,op):""};
        }
        if(current=='-'&&pos+1<input.length()&&input[pos+1]=='>'){
            pos+=2;
            return{OPERATOR,"->",mode=="general"?getTokenName(OPERATOR,"->"):""};
        }
        if(current==';'||current=='('||current==')'||current=='{'||current=='}'||
           current=='['||current==']'||current==','||current=='.'||current==':'||
           current=='?'||current=='@'){
            string sym=string(1,current);
            pos++;
            if(current==':'&&pos<input.length()&&input[pos]==':'){
                sym+=input[pos++];
            }
            return{SYMBOL,sym,mode=="general"?getTokenName(SYMBOL,sym):""};
        }
        return{ERROR,string(1,input[pos++]),mode=="general"?getTokenName(ERROR,string(1,current)):""};
    }

    vector<Token> tokenize(){
        vector<Token> tokens;
        Token token;
        do{
            token=nextToken();
            if(mode=="compiler"&&(token.type==WHITESPACE||token.type==COMMENT)){
                continue;
            }
            if(token.type!=END){
                tokens.push_back(token);
            }
        }while(token.type!=END);
        return tokens;
    }
};

string tokenTypeToString(TokenType type){
    switch(type){
        case KEYWORD:return "KEYWORD";
        case IDENTIFIER:return "IDENTIFIER";
        case NUMBER:return "NUMBER";
        case OPERATOR:return "OPERATOR";
        case SYMBOL:return "SYMBOL";
        case COMMENT:return "COMMENT";
        case WHITESPACE:return "WHITESPACE";
        case PREPROCESSOR:return "PREPROCESSOR";
        case STRING:return "STRING";
        case ERROR:return "ERROR";
        case END:return "END";
        default:return "UNKNOWN";
    }
}

int main(){
    string input;
    int input_choice,mode_choice;
    cout<<"Enter 1 for inline code or 2 for file input:";
    cin>>input_choice;
    cin.ignore();
    if(input_choice==1){
        cout<<"Enter C++ code:";
        getline(cin,input);
    }else if(input_choice==2){
        cout<<"Enter .cpp file path:";
        string filepath;
        getline(cin,filepath);
        ifstream file(filepath);
        if(!file.is_open()){
            cout<<"Error: Cannot open file"<<endl;
            return 1;
        }
        string line;
        while(getline(file,line)){
            input+=line+"\n";
        }
        file.close();
    }else{
        cout<<"Invalid input choice"<<endl;
        return 1;
    }
    cout<<"Enter 1 for compiler mode or 2 for general mode:";
    cin>>mode_choice;
    cin.ignore();
    string mode=(mode_choice==1)?"compiler":(mode_choice==2)?"general":"";
    if(mode.empty()){
        cout<<"Invalid mode choice"<<endl;
        return 1;
    }
    Lexer lexer(input,mode);
    vector<Token> tokens=lexer.tokenize();
    for(auto token:tokens){
        if(mode=="general"){
            cout<<"Token:"<<tokenTypeToString(token.type)<<", Value:"<<token.value<<", Name:"<<token.name<<endl;
        }else{
            cout<<"Token:"<<tokenTypeToString(token.type)<<", Value:"<<token.value<<endl;
        }
    }
    cout<<"Total number of tokens:"<<tokens.size()<<endl;
    return 0;
}
extern "C"{
    const char* analyze(const char* input, const char* modeStr) {
        static string result;
        string mode=string(modeStr);
        Lexer lexer(input, mode);
        vector<Token> tokens=lexer.tokenize();
        result.clear();
        for(const auto& token:tokens){
            result+= "Token: "+tokenTypeToString(token.type)
                    + ", Value: "+token.value
                    +(mode=="general"?(",Name: "+token.name):"")+"\n";
        }
        result+="Total number of tokens: "+to_string(tokens.size())+"\n";
        return result.c_str();
    }
}

