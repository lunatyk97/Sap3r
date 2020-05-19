#pragma once
class Col
{
public:
    int r;
    int g;
    int b;

    Col();
    Col(int r, int g, int b);

    int checkColor(int val);
};

