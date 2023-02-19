#include <iostream>
#include <algorithm>
#include <sstream>
#include <functional>
#include <thread>
#include <Windows.h>

/*-------------------------------------------------------------------------*/
///////////////  БЛОК ИНИЦИАЛИЗАЦИИ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ  //////////////////

bool play_g;  // переменная флаг, отвечает за проигрывание музыки. Если true, то воспроизводит звук после каждого отпускания кнопки
int pause_time_g;    // время воспроизведения звука
int frequency_g;     // частота звука

/*-------------------------------------------------------------------------*/
//////////////////////  БЛОК ОБЪЯВЛЕНИЯ ФУНКЦИЙ  ////////////////////////////

template <typename To, typename From> To toInt(From&& from);
inline void playSound();
[[nodiscard]] std::pair<int, int> init(int argc, char* argv[]);
void showInfo();
inline bool isCommandCompleted(DWORD cmd);

LRESULT CALLBACK hookKeyBourd(int code, WPARAM wParam, LPARAM lParam);

/*-------------------------------------------------------------------------*/
/////////////////////////////  ТОЧКА ВХОДА  /////////////////////////////////

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    std::tie(pause_time_g, frequency_g) = init(argc, argv);

    showInfo();

    SetWindowsHookEx(WH_KEYBOARD_LL, hookKeyBourd, 0, 0);

    
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) != 0) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

/*-----------------------------------------------------------------------------*/
/////////////////////////  БЛОК ОПРЕДЕЛЕНИЙ ФУНКЦИЙ  ////////////////////////////

/// <summary>
/// инициализация глобальных переменных.
/// данные берутся из командной строки, если она есть.
/// </summary>
/// <returns></returns>
std::pair<int, int> init(int argc, char* argv[]) {
    if (argc == 3) {
        const int min_pause_time{};
        const int min_frequency{ 37 };
        const int max_pause_time{ 10000 };
        const int max_frequency{ 32767 };
        auto pause_time{ toInt<int>(argv[1]) };
        auto frequency{ toInt<int>(argv[2]) };

        pause_time = std::clamp(pause_time, min_pause_time, max_pause_time);
        frequency = std::clamp(frequency, min_frequency, max_frequency);
        return { pause_time , frequency };
    }
    return { 200, 300 };
}

/*-------------------------------------------------------------------------*/
/// <summary>
/// Вывод информационных данных на стандартный вывод
/// </summary>
void showInfo() {
    system("cls");
    std::cout << "Текущие данные:\n\n" 
        "статус звука нажатий клавиш:\t" << (play_g ? "Включено" : "Выключено") << "\n"
        << "Частота:\t\t\t" << frequency_g << " Гц.\n"
        << "Время воспроизведения:\t\t" << pause_time_g << " мс.\n\n\n"
        << "Для включения/выключения: одновременно нажмите клавиши \"SHIFT\" и \"ESCAPE\"\n"
        << "Для именения значений, передайте параметры в качестве аргументов командной строки\n\n"
        << "Пример:\nпуть к программе/Handling_keyboard_interrupts.exe 200 1000\n\n"
        << "где,\n1000 - частота в Гц.\n200 - время в мс."
        << std::endl;
}

/*-------------------------------------------------------------------------*/
/// <summary>
/// В случае одновременного нажатия клавиш SHIFT и ESCAPE, возвращает true, иначе false
/// </summary>
 bool isCommandCompleted(DWORD cmd) {

    static bool is_shift_down{};
    if (cmd == VK_LSHIFT || cmd == VK_RSHIFT) {
        is_shift_down = is_shift_down ? false : true;
        return false;
    }
    else if (is_shift_down && cmd == VK_ESCAPE) return true;
    
    is_shift_down = false;
    return false;
}

/*-------------------------------------------------------------------------*/
/// <summary>
/// Принимает управление в случае нажатия на любую кнопку клавиатуры
/// </summary>
LRESULT CALLBACK hookKeyBourd(int code, WPARAM wParam, LPARAM lParam) {

    if (code == HC_ACTION) {
        auto code{ reinterpret_cast<LPKBDLLHOOKSTRUCT>(lParam)->vkCode };
        if (wParam == WM_KEYDOWN && isCommandCompleted(code)) {
            play_g = !play_g;
            showInfo();
        }
        else if (wParam == WM_KEYUP && play_g) playSound();
    }
    return CallNextHookEx(0, code, wParam, lParam);
}

/*-------------------------------------------------------------------------*/
/// <summary>
/// Создает поток, в котором возпроизводится звук.
/// Поток умирает после проигрывания.
/// Не ждет пока выполнится поток.
/// </summary>
void playSound() {
    std::thread(std::bind(Beep, frequency_g, pause_time_g)).detach();
}

/*-------------------------------------------------------------------------*/
/// <summary>
/// Конвертирует из типа From в тип To.
/// Если провал, то возвращает данные, созданные конструктором по умолчанию.
/// Если нет конструктора по умолчанию, то ошибка компиляции.
/// NRVO
/// </summary>
template <typename To, typename From>
To toInt(From&& from) {
    To to{};
    std::stringstream ss;
    ss << std::forward<From>(from);
    ss >> to;
    return to;
}