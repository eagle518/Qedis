#ifndef BERT_PROTOCOL_H
#define BERT_PROTOCOL_H

#include <vector>
#include <string>

enum class ParseResult : int8_t 
{ 
    ok, 
    wait, 
    error,
};

class ServerProtocol
{
public:
    void Reset();
    ParseResult ParseRequest(const char*& ptr, const char* end);

    const std::vector<std::string>& GetParams() const { return params_; }
    const std::string GetRawRequest() const { return content_; }
    void SetParams(const std::vector<std::string>& params) { params_ = params; }
    void SetParam(const std::string& param) { content_ = param; }
    
    bool IsInitialState() const { return multi_ == -1; }

private:
    ParseResult _ParseMulti(const char*& ptr, const char* end, int& result);
    ParseResult _ParseStrlist(const char*& ptr, const char* end, std::vector<std::string>& results);
    ParseResult _ParseStr(const char*& ptr, const char* end, std::string& res);
    ParseResult _ParseStrval(const char*& ptr, const char* end, std::string& res);
    ParseResult _ParseStrlen(const char*& ptr, const char* end, int& result);

    int multi_ = -1;
    int paramLen_ = -1;
    std::vector<std::string> params_;
    int nParams_ = 0;
    std::string content_;
};


enum class ResponseType
{
    None,
    Fine,   // +
    Error,  // -
    String, // $
    Number, // :
    Multi,  // *
};

class ClientProtocol
{
public:
    ClientProtocol();

    void Reset();

    ParseResult Parse(const char*& data, const char* end);
    const std::string& GetParam() const { return content_; }

private:
    // $3\r\nabc\r\n
    ParseResult _ParseStr(const char*& ptr, const char* end);
    ParseResult _ParseStrval(const char*& ptr, const char* end);
    ParseResult _ParseStrlen(const char*& ptr, const char* end, int& result);

    // *2\r\n$4\r\nname\r\n$4\r\nbert\r\n
    ParseResult _ParseMulti(const char*& ptr, const char* end);

    static const int kInvalid = -2;

    ResponseType type_;
    std::string content_;
    int multi_ = kInvalid;
    int paramLen_ = kInvalid;
    int nParams_ = 0;
};


#endif

