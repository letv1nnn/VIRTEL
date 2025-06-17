
use std::collections::HashMap;


#[derive(Clone)]
#[derive(Debug, PartialEq)]
pub enum TokenType {
    LeftParen, RightParen,
    LeftBrace, RightBrace,
    Comma, Minus, Plus,
    RightShift, LeftShift,
    Semicolon, Slash, Star,
    Bang, BangEqual,
    Equal, EqualEqual,
    Greater, GreaterEqual,
    Less, LessEqual,
    Identifier, Str, Number,
    And, Else, False,
    Fun, If, Noth, For,
    Print, Return, Or,
    Var, While, True,
    Error, Eof
}

#[derive(Debug)]
pub struct Token {
    pub token_type: TokenType,
    pub lexeme: String,
    pub line: usize,
}

pub struct Scanner {
    source: String,
    start: usize,
    current: usize,
    line: usize,
    keywords: HashMap<String, TokenType>,
}

impl Scanner {
    pub fn new(source: String) -> Self {
        let mut keywords = HashMap::new();
        keywords.insert("if".to_string(), TokenType::If);
        keywords.insert("else".to_string(), TokenType::Else);
        keywords.insert("for".to_string(), TokenType::For);
        keywords.insert("while".to_string(), TokenType::While);
        keywords.insert("and".to_string(), TokenType::And);
        keywords.insert("or".to_string(), TokenType::Or);
        keywords.insert("true".to_string(), TokenType::True);
        keywords.insert("false".to_string(), TokenType::False);
        keywords.insert("noth".to_string(), TokenType::Noth);
        keywords.insert("fun".to_string(), TokenType::Fun);
        keywords.insert("var".to_string(), TokenType::Var);
        keywords.insert("print".to_string(), TokenType::Print);
        keywords.insert("return".to_string(), TokenType::Return);

        Scanner {
            source,
            start: 0,
            current: 0,
            line: 1,
            keywords,
        }
    }

    pub fn scan_token(&mut self) -> Token {
        self.skip_whitespace();
        self.start = self.current;

        if self.is_at_end() {
            self.make_token(TokenType::Eof);
        }

        let c = self.advance();

        match c {
            '+' => self.make_token(TokenType::Plus),
            '-' => self.make_token(TokenType::Minus),
            '*' => self.make_token(TokenType::Star),
            '/' => self.make_token(TokenType::Slash),
            '(' => self.make_token(TokenType::LeftParen),
            ')' => self.make_token(TokenType::RightParen),
            '{' => self.make_token(TokenType::LeftBrace),
            '}' => self.make_token(TokenType::RightBrace),
            ',' => self.make_token(TokenType::Comma),
            ';' => self.make_token(TokenType::Semicolon),
            '"' => self.string(),
            c if c.is_ascii_digit() => self.numbers(),
            c if c.is_ascii_alphabetic() => self.identifier(),

            // need to handle tokens with length more than one like < or <=, etc.
            '=' => {
                let exp = self.match_char('=');
                match exp {
                    true => self.make_token(TokenType::EqualEqual),
                    false => self.make_token(TokenType::Equal),
                }
            },
            '!' => {
                let exp = self.match_char('=');
                match exp {
                    true => self.make_token(TokenType::BangEqual),
                    false => self.make_token(TokenType::Bang),
                }
            },
            '<' => {
                // introducing left shifting here
                let exp = self.source.chars().nth(self.current).unwrap();
                self.current += 1;
                match exp {
                    '=' => self.make_token(TokenType::LessEqual),
                    '<' => self.make_token(TokenType::LeftShift),
                    _ => self.make_token(TokenType::Less),
                }
            },
            '>' => {
                let exp = self.source.chars().nth(self.current).unwrap();
                self.current += 1;
                match exp {
                    '=' => self.make_token(TokenType::GreaterEqual),
                    '>' => self.make_token(TokenType::RightShift),
                    _ => self.make_token(TokenType::Greater),
                }
            }

            _ => self.make_token(TokenType::Error),
        }
    }

    fn is_at_end(&self) -> bool {
        self.current >= self.source.len()
    }

    fn advance(&mut self) -> char {
        let c = self.source.chars().nth(self.current).unwrap();
        self.current += 1;
        c
    }

    fn match_char(&mut self, expected: char) -> bool {
        if self.is_at_end() {
            return false;
        }
        if self.source.chars().nth(self.current) != Some(expected) {
            return false;
        }
        self.current += 1;
        true
    }

    fn string(&mut self) -> Token {
        while let Some(c) = self.source.chars().nth(self.current) {
            if c == '"' {
                self.advance();
                return self.make_token(TokenType::Str);
            }
            if c == '$' {
                if self.match_char('{') {
                    self.identifier();
                }
                if self.advance() != '}' {
                    return self.make_token(TokenType::Error);
                }
            }
            if c == '\n' {
                self.line += 1;
            }
            self.advance();
        }

        self.make_token(TokenType::Error)
    }

    fn numbers(&mut self) -> Token {
        while let Some(c) = self.source.chars().nth(self.current) {
            if !c.is_ascii_digit() {
                break;
            }
            self.advance();
        }
        // Look for fractional part
        if let Some('.') = self.source.chars().nth(self.current) {
            if let Some(c) = self.source.chars().nth(self.current + 1) {
                if c.is_ascii_digit() {
                    self.advance();
                    while let Some(c) = self.source.chars().nth(self.current) {
                        if !c.is_ascii_digit() {
                           break;
                        }
                        self.advance();
                    }
                }
            }
        }
        self.make_token(TokenType::Number)
    }

    // need to  also handle the keywords recognition, like for/false or the variable
    fn identifier(&mut self) -> Token {
        while let Some(c) = self.source.chars().nth(self.current) {
            if !c.is_ascii_alphabetic() && c != '_' {
                break;
            }
            self.advance();
        }
        let text = &self.source[self.start..self.current];
        match self.keywords.get(text) {
            Some(token_type) => self.make_token(token_type.clone()),
            None => self.make_token(TokenType::Identifier),
        }
    }

    fn skip_whitespace(&mut self) {
        loop {
            let curr = self.source.chars().nth(self.current);
            match curr {
                Some(' ') => self.advance(),
                Some('\t') | Some('\r') => self.advance(),
                Some('\n') => {
                    self.line += 1;
                    self.advance()
                },
                Some('/') => {
                    if let Some('/') = self.source.chars().nth(self.current + 1) {
                        // A comment goes until the end of the line
                        while let Some(c) = self.source.chars().nth(self.current) {
                            if c == '\n' {
                                break;
                            }
                            self.advance();
                        } return
                    } else {
                        break;
                    }
                },
                _ => return,
            };
        }
    }
    // returns a token with a specific type and
    // start____current slice of the string code
    fn make_token(&self, token_type: TokenType) -> Token {
        let lexeme = self.source[self.start..self.current].to_string();
        Token {
            token_type,
            lexeme,
            line: self.line,
        }
    }
}

// temprorary testing
fn main() {
    let mut source = String::from("");
    std::io::stdin().read_line(&mut source).expect("Failed to read the line!");

    let mut scanner = Scanner::new(source);
    loop {
        let token = scanner.scan_token();
        println!("{:?}", token);
        if token.token_type == TokenType::Eof {
            break;
        }
    }
}

