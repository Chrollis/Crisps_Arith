#include "next.h"

int WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd) {
    SceneController game;
    FpsController fps_controller;
    ExMessage msg;
    clock_t last_tick = clock();
    clock_t current_tick = clock();

    initgraph(x_screen, y_screen);
    fps_controller.set(fps);

    BeginBatchDraw();

    game.set(SceneController::SLogin);

    while (1) {
        fps_controller.start();

        while (peekmessage(&msg)) {
            game.input(msg);
        }

        current_tick = clock();
        game.proceed(current_tick - last_tick);
        last_tick = current_tick;

        cleardevice();
        game.draw();
        fps_controller.draw();

        if (is_ended) {
            if (current_mode == "caidan") {
                game.switch_to(SceneController::SMainMenu);
            }
            else if (current_mode == "kousuan") {
                game.switch_to(SceneController::SLogin);
            }
            else if (current_mode == "kousuan_A" ||
                current_mode == "kousuan admin") {
                game.switch_to(SceneController::SAdmin);
            }
            else if (current_mode == "kousuan_B") {
                game.switch_to(SceneController::SArithmetic);
            }
            else if (current_mode == "tuichu") {
                exit(0);
            }
        }

        FlushBatchDraw();

        if (is_ended) {
            is_ended = 0;
        }
        else {
            fps_controller.delay();
        }
    }
    EndBatchDraw();

    closegraph();
    return 0;
}