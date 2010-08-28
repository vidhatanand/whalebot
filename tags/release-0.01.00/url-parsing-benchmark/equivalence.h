#pragma once

#include <vector>

#include "parser_type.h"
#include "answer.h"

typedef std::vector<eUrlParsers>        TEquivalenceClass;
typedef std::vector<TEquivalenceClass>  TEquivalenceRelation;



TEquivalenceRelation gFindRelated( const TParsersResults& results );
