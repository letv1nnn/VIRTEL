
use std::collections::HashMap;

pub enum TokenType {
    LeftParen, RightParen,
    LeftBrace, RightBrace,
    Comma, Minus, Plus,
    Semicolon, Slash, Star,

    Bang, BangEqual,
    Equal, EqualEqual,
    Greater, GreaterEqual,
    Less, LessEqual,

    Identifier, Str, Number,

    And, Else, False,
    Fun, If, Noth, For,
    Print, Return, Or,
    Var, While, True

    Error, Eof
}


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
        keywords.insert("noth".to_string(), TokenType::Nil);
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
        self.skip_whitspace();
        self.start = self.current;

        if is_at_end() {
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
            ',' => self.make_token(TokenType::Comma).
            ';' => self.make_token(TokenType::Semicolon),

            // need to handle tokens with length more than one like < or <=, etc.
            '=' => {
                let exp = self.match_char('=');
                match exp {
                    true: self.make_token(TokenType::EqualEqual),
                    false: self.make_token(TokenType::Equal),
                }
            },
            '!' => {
                let exp = self.match_char('=');
                match exp {
                    true: self.make_token(TokenType::BangEqual),
                    false: self.make_token(TokenType::Bang),
                }
            },
            '<' => {
                let exp = self.match_char('=');
                match exp {
                    true: self.make_token(TokenType::LessEqual),
                    false: self.make_token(TokenType::Less),
                }
            },
            '>' => {
                let exp = self.match_char('=');
                match exp {
                    true: self.make_token(TokenType::GreaterEqual),
                    false: self.make_token(TokenType::Greater),
                }
            }

            _ => self.make_token(TokenType::Eof),
        }
    }

    fn is_at_end(&self) -> bool {
        return self.current >= self.source.len();
    }

    fn advance(&mut self) -> char {
        self.current += 1;
        self.source.chars().nth(self.current - 1).unwrap()
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

    fn skip_whitespace(&mut self) {
        loop {
            let curr = self.source.chars().nth(self.current);
            match curr {
                ' ' => self.advance(),
                '\t' | '\r' => self.advance(),
                '\n' => {
                    self.advance();
                    self.line += 1;
                },
                '/' => {
                    let is_comment = self.match('/');
                    match is_comment {
                        true => {
                            while self.source.chars().nth(self.current) != '\n' || !self.is_at_end() {
                                self.advance();
                            }
                        },
                        false => return;
                    }
                },
                '_' => return,
            }
        }
    }

    fn make_token(&self, token_type: TokenType) -> Token {
        let lexeme = self.source[self.start..self.current].to_string();
        Token {
            token_type,
            lexeme,
            line: self.line,
        }
    }
}

