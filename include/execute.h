#ifndef EXECUTE_H
#define EXECUTE_H

#include "parser.h"

/*
 * Execute one command.
 *
 * Returns:
 *      0  -> success
 *     -1  -> error
 */
int execute_command(Command *cmd);

#endif
