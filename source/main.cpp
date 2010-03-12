#include <nds.h>
#include "draw.h"
#include "graph.h"
#include <vector>
#include <stdio.h>
#include <math.h>

#define X_OFFSET SCREEN_WIDTH/2
#define Y_OFFSET SCREEN_HEIGHT/2

void ClearScreen(void)
{
    for(int i = 0; i < 256 * 192; i++)
	VRAM_A[i] = RGB15(0,0,0);
}

int main()
{
    touchPosition touch;

    defaultExceptionHandler();

    lcdMainOnBottom();
    irqEnable(IRQ_VBLANK);

    videoSetMode(MODE_FB0);
    vramSetBankA(VRAM_A_LCD);

    consoleDemoInit();

    ClearScreen();

    touchPosition firstTouch = { 0, 0 };
    touchPosition lastTouch = { 0, 0 };

    int colours[3] = { 31, 31, 31 };

    PointVector points;
    NodeVector nodes;

    //we like infinite loops in console dev!
    while(1)
    {
	swiWaitForVBlank();

	scanKeys();
	touchRead(&touch);

	int kd = keysDown();
	if (kd & KEY_A && points.size() > 1)
	{
	    ClearScreen();

	    for (size_t i = 0; i < points.size(); ++i)
            {
                draw::plot(points[i]->X+X_OFFSET,points[i]->Y+Y_OFFSET,RGB15(colours[0],colours[1],colours[2]));
            }

	    printf("Working!\n");

	    size_t off = 1;

            printf("Building difference tree.\n");
            for (size_t i = 0; i < points.size(); ++i)
            {
                nodes.push_back(NodePtr(new Node(points[i])));
            }
            for (size_t i = 0; i < points.size()-1; ++i)
            {
                size_t size = nodes.size();
                for (size_t j = off; j < size; ++j)
                {
                    nodes.push_back(NodePtr(new Node(nodes[j-1],nodes[j])));
                }
                off += points.size() - i;
            }

            printf("Finding coefficients.\n");
            NodePtr n = nodes[nodes.size()-1];
            std::vector<double> coef;
            while (n.get() != NULL)
            {
                coef.push_back(n->difference());
                n = n->left;
            }

            for (size_t i = 0; i < coef.size(); ++i)
            {
                printf("%f\n",coef[i]);
            }

            double lx = -SCREEN_WIDTH/2;
            double ly = evaluateFunction(lx, coef, points);

            printf("Evaluating polynomial.\n");
            for (double x = -SCREEN_WIDTH/2; x < SCREEN_WIDTH/2; x += 1)
            {
                double y = evaluateFunction(x, coef, points);

                printf("%f %f\n",x,y);
                draw::line(x+X_OFFSET, Y_OFFSET+y, lx+X_OFFSET, Y_OFFSET+ly);
                lx = x;
                ly = y;
            }

            printf("Done!\n");

            points.clear();
            nodes.clear();
	}

	if (kd & KEY_B)
	{
	    ClearScreen();
	}

	if(kd & KEY_TOUCH)
	{
	    firstTouch = touch;
            draw::plot(touch.px,touch.py,RGB15(colours[0],colours[1],colours[2]));
            points.push_back(PointPtr(new Point(touch.px-X_OFFSET, touch.py-Y_OFFSET)));
	}

	if(keysHeld() & KEY_TOUCH)
	{
            lastTouch = touch;
            draw::line(firstTouch.px, firstTouch.py, lastTouch.px, lastTouch.py, RGB15(colours[0],colours[1],colours[2]));
            firstTouch = touch;
	}
    }

    return 0;
}
