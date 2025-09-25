//
// Created by Jesse on 24-11-2023.
//

#ifndef WINDOWCONSOLE_H
#define WINDOWCONSOLE_H

void BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr);
void AttachWindowsConsole();

#endif //WINDOWCONSOLE_H
