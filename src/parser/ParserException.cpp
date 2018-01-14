#include <exception>

class ParserException : std::exception {
public:
	explicit ParserException(const char* msg) {
		this->msg = msg;
	}

	char const* what() const override {
		return msg;
	}

private:
	const char* msg;
};
