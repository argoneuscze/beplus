#pragma once

#include "ast/ASTNode.h"

const bool IS_DEBUG = true;

void LogParsingError(const char* msg);
void LogDebug(const char* msg);
void LogDebug(const ASTNode* node);
