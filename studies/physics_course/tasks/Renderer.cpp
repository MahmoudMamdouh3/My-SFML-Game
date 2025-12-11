#include "headers/Renderer.h"

void Renderer::AddDrawable(Drawable* drawable)
{
    drawables.push_back(drawable);
}

void Renderer::Render(RenderWindow* window)
{
    window->clear();

    list<Drawable*>::iterator it;
    for (it = drawables.begin(); it != drawables.end(); it++)
    {
        window->draw(**it);
    }

    window->display();
}