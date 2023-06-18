#include <parser.h>

Parser::Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens))
{
}

std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!is_at_end()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::declaration()
{
    try {
        if (match(TokenType::Fn)) {
            return function();
        } else if (match(TokenType::Let)) {
            return var_declaration();
        }
        return statement();
    } catch (ParseError err) {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::var_declaration()
{
    auto name = consume(TokenType::Identifier, "expected a variable name");
    std::unique_ptr<Expr> initializer;
    if (match(TokenType::Equal)) {
        initializer = expression();
    }
    consume(TokenType::Semicolon, "expected ';' after variable declaration");
    return std::make_unique<VarStmt>(name, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::function()
{
    auto name = consume(TokenType::Identifier, "expected function name");
    consume(TokenType::LeftParen, "expected a parenthesis after function declaration");

    std::vector<Token> parameters;
    if (!check(TokenType::RightParen)) {
        do {
            parameters.push_back(consume(TokenType::Identifier, "expected argument to have a name"));
        } while (match(TokenType::Comma));
    }
    // consume unnecessary characters
    consume(TokenType::RightParen, "Expect ')' after parameters.");
    consume(TokenType::LeftBrace, "expected '{' to initialize function body");

    std::vector<std::unique_ptr<Stmt>> body = block();
    return std::make_unique<FunctionStmt>(name, std::move(parameters), std::move(body));
}

std::unique_ptr<Stmt> Parser::statement()
{
    if (match(TokenType::For)) {
        return for_statement();
    }
    if (match(TokenType::If)) {
        return if_statement();
    }
    if (match(TokenType::Print)) {
        return print_statement();
    }
    if (match(TokenType::Return)) {
        return return_statement();
    }
    if (match(TokenType::While)) {
        return while_statement();
    }
    if (match(TokenType::LeftBrace)) {
        return std::make_unique<BlockStmt>(block());
    }

    return expression_statement();
}

std::unique_ptr<Stmt> Parser::for_statement()
{
    consume(TokenType::LeftParen, "Expect '(' after 'for'.");

    std::unique_ptr<Stmt> initializer;
    if (match(TokenType::Semicolon)) {
        initializer = nullptr;
    } else if (match(TokenType::Let)) {
        initializer = var_declaration();
    } else {
        initializer = expression_statement();
    }

    std::unique_ptr<Expr> condition;
    if (!check(TokenType::Semicolon)) {
        condition = expression();
    }
    consume(TokenType::Semicolon, "Expect ';' after loop condition.");

    std::unique_ptr<Expr> increment;
    if (!check(TokenType::RightParen)) {
        increment = expression();
    }
    consume(TokenType::RightParen, "Expect ')' after for clauses.");

    auto body = statement();

    // basically the code below code just converts the for loop
    // into a weird version of a while loop
    if (increment) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(body));
        statements.push_back(std::make_unique<ExpressionStmt>(std::move(increment)));
        body = std::make_unique<BlockStmt>(std::move(statements));
    }

    if (!condition) {
        condition = std::make_unique<LiteralExpr>(true);
    }
    body = std::make_unique<WhileStmt>(std::move(condition), std::move(body));

    if (initializer) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<BlockStmt>(std::move(statements));
    }

    return body;
}

std::unique_ptr<Stmt> Parser::if_statement()
{
    consume(TokenType::LeftParen, "Expected '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RightParen, "Expected ')' after 'if'.");

    auto then_branch = statement();
    std::unique_ptr<Stmt> else_branch;
    if (match(TokenType::Else)) {
        else_branch = statement();
    }

    return std::make_unique<IfStmt>(std::move(condition), std::move(then_branch),
        std::move(else_branch));
}

std::unique_ptr<Stmt> Parser::print_statement()
{
    auto value = expression();
    consume(TokenType::Semicolon, "Expect ';' after value");
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::return_statement()
{
    auto keyword = previous();
    std::unique_ptr<Expr> value;
    if (!check(TokenType::Semicolon)) {
        value = expression();
    }

    consume(TokenType::Semicolon, "Expect ';' after return value");
    return std::make_unique<ReturnStmt>(std::move(keyword), std::move(value));
}

std::unique_ptr<Stmt> Parser::while_statement()
{
    consume(TokenType::LeftParen, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RightParen, "Expect ')' after 'while'.");

    return std::make_unique<WhileStmt>(std::move(condition), statement());
}

std::vector<std::unique_ptr<Stmt>> Parser::block()
{
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(TokenType::RightBrace) && !is_at_end()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RightBrace, "Expect '}' after block.");
    return statements;
}

std::unique_ptr<Stmt> Parser::expression_statement()
{
    auto expr = expression();
    consume(TokenType::Semicolon, "Expect ';' after expression");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

std::unique_ptr<Expr> Parser::expression()
{
    return assignment();
}

std::unique_ptr<Expr> Parser::assignment()
{
    auto expr = or_expression();

    if (match(TokenType::Equal)) {
        auto equals = previous();
        auto value = assignment();

        if (auto* var_expr = dynamic_cast<VarExpr*>(expr.get()); var_expr) {
            return std::make_unique<AssignExpr>(var_expr->get_name(), std::move(value));
        }

        error(equals, "Invalid assignment target");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::or_expression()
{
    auto expr = and_expression();

    while (match(TokenType::Or)) {
        auto op = previous();
        auto right = and_expression();
        expr = std::make_unique<LogicalExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::and_expression()
{
    auto expr = equality();

    while (match(TokenType::And)) {
        auto op = previous();
        auto right = equality();
        expr = std::make_unique<LogicalExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::equality()
{
    auto expr = comparison();
    while (match(TokenType::BangEqual) || match(TokenType::EqualEqual)) {
        auto op = previous();
        auto right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison()
{
    auto expr = addition();
    while (
        match(TokenType::Greater) || match(TokenType::GreaterEqual) || match(TokenType::Less) || match(TokenType::LessEqual)) {
        auto op = previous();
        auto right = addition();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::addition()
{
    auto expr = multiplication();
    while (match(TokenType::Minus) || match(TokenType::Plus)) {
        auto op = previous();
        auto right = multiplication();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::multiplication()
{
    auto expr = unary();
    while (match(TokenType::Slash) || match(TokenType::Star)) {
        auto op = previous();
        auto right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary()
{
    if (match(TokenType::Bang) || match(TokenType::Minus)) {
        auto op = previous();
        auto right = unary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }

    return call();
}

std::unique_ptr<Expr> Parser::call()
{
    auto expr = primary();
    while (true) {
        if (match(TokenType::LeftParen)) {
            expr = finish_call(std::move(expr));
        } else {
            break;
        }
    }
    return expr;
}

std::unique_ptr<Expr> Parser::finish_call(std::unique_ptr<Expr> callee)
{
    std::vector<std::unique_ptr<Expr>> arguments;

    if (!check(TokenType::RightParen)) {
        do {
            arguments.push_back(expression());
        } while (match(TokenType::Comma));
    }

    auto paren = consume(TokenType::RightParen, "Expect ')' after the arguments.");
    return std::make_unique<CallExpr>(std::move(callee), paren, std::move(arguments));
}

std::unique_ptr<Expr> Parser::primary()
{
    if (match(TokenType::Number) || match(TokenType::String)) {
        return std::make_unique<LiteralExpr>(previous().get_literal());
    }

    if (match(TokenType::False)) {
        return std::make_unique<LiteralExpr>(false);
    }

    if (match(TokenType::True)) {
        return std::make_unique<LiteralExpr>(true);
    }

    if (match(TokenType::Null)) {
        return std::make_unique<LiteralExpr>(std::any {});
    }

    if (match(TokenType::Identifier)) {
        return std::make_unique<VarExpr>(previous());
    }

    if (match(TokenType::LeftParen)) {
        auto expr = expression();
        consume(TokenType::RightParen, "Expect ')' after expression");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

bool Parser::check(TokenType type) const
{
    if (is_at_end()) {
        return false;
    }
    return peek().get_type() == type;
}

Token Parser::advance()
{
    if (!is_at_end()) {
        ++m_current;
    }
    return previous();
}

Token Parser::peek() const
{
    return m_tokens.at(m_current);
}

Token Parser::previous() const
{
    return m_tokens.at(m_current - 1);
}

bool Parser::is_at_end() const
{
    return peek().get_type() == TokenType::TokenEOF;
}

void Parser::synchronize()
{
    advance();

    while (!is_at_end()) {
        if (previous().get_type() == TokenType::Semicolon) {
            return;
        }

        switch (peek().get_type()) {
        case TokenType::Class:
        case TokenType::Fn:
        case TokenType::Let:
        case TokenType::For:
        case TokenType::If:
        case TokenType::While:
        case TokenType::Print:
        case TokenType::Return:
            return;
        default:
            break;
        }

        advance();
    }
}

Token Parser::consume(TokenType type, const char* message)
{
    if (check(type)) {
        return advance();
    }

    throw error(peek(), message);
}

Parser::ParseError Parser::error(Token token, const char* message) const
{
    // Lox::Error(token, message);
    std::cout << "error occured :: " << token.get_text() << " -> " << message << "\n";
    throw ParseError {};
}

bool Parser::match(TokenType type)
{
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}
