#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cassert>

void* Hook(void* functionToHook, void* myFunction, unsigned size, bool skip_virtualprotects = false)
{
  assert(functionToHook);
  assert(size > 0);
  assert(myFunction);
  DWORD old, old2;
  LPVOID oldInstructions = VirtualAlloc(0, size + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  assert(oldInstructions);
  memcpy(oldInstructions, functionToHook, size);
  *(BYTE*)((DWORD)oldInstructions + size) = 0xE9;
  *(DWORD*)((DWORD)oldInstructions + size + 1) = (DWORD)((DWORD)functionToHook + size) - (DWORD)((DWORD)oldInstructions + size) - 5;
  if (!skip_virtualprotects)
  {
    bool vpres = VirtualProtect(functionToHook, size, PAGE_READWRITE, &old);
    assert(vpres);
  }
  *(BYTE*)functionToHook = 0xE9;
  *(DWORD*)((DWORD)functionToHook + 1) = (DWORD)myFunction - (DWORD)functionToHook - 5;
  if (!skip_virtualprotects)
  {
    bool vpres = VirtualProtect(functionToHook, size, old, &old2);
    assert(vpres);
  }
  return oldInstructions;
}

void(__cdecl* realDataRecv)(int id, const char buf[], int len, int zero);
void __cdecl myDataRecv(int id, const char buf[], int len, int zero)
{
  for (int i = 0; i < len; ++i)
    printf("%02X ", (unsigned char)buf[i]);
  putchar('\n');
  return realDataRecv(id, buf, len, zero);
}

void encrypt(char* in, int size)
{
  char XorBytes[] = { '\xAB', '\x11', '\xCD', '\xFE', '\x18', '\x23', '\xC5', '\xA3', '\xCA', '\x33', '\xC1', '\xCC', '\x66', '\x67', '\x21', '\xF3', '\x32', '\x12', '\x15', '\x35', '\x29', '\xFF', '\xFE', '\x1D', '\x44', '\xEF', '\xCD', '\x41', '\x26', '\x3C', '\x4E', '\x4D' };
  for (int i = 3; i < size; i++)
    in[i] = in[i] ^ XorBytes[i % sizeof(XorBytes)] ^ in[i - 1];
}

void decrypt(char* in, int size)
{
  char XorBytes[] = { '\xAB', '\x11', '\xCD', '\xFE', '\x18', '\x23', '\xC5', '\xA3', '\xCA', '\x33', '\xC1', '\xCC', '\x66', '\x67', '\x21', '\xF3', '\x32', '\x12', '\x15', '\x35', '\x29', '\xFF', '\xFE', '\x1D', '\x44', '\xEF', '\xCD', '\x41', '\x26', '\x3C', '\x4E', '\x4D' };
  for (int i = size - 1; i > 2; i--)
    in[i] = in[i] ^ XorBytes[i % sizeof(XorBytes)] ^ in[i - 1];
}

volatile unsigned additional_hits = 0;
volatile bool enabled_for_hand = false;
volatile bool enabled_for_spell = false;

void(__fastcall* realDataSend)(char pakjetZDwoma[], void* fastcallDummy, int zerolubjeden, int zero);
void __fastcall myDataSend(char pakjetZDwoma[], void* fastcallDummy, int zerolubjeden, int zero)
{
  //na przyklad
  //07 00 C1 07 D7 AC 15 64 F4 00 00 00
  //07 00 C1 01 19 E7 FF FC 3A 00
  if (pakjetZDwoma[2] != '\xC1')
    return realDataSend(pakjetZDwoma, fastcallDummy, zerolubjeden, zero);
  if (pakjetZDwoma[4] != '\x19' && pakjetZDwoma[4] != '\x1E' && pakjetZDwoma[4] != '\xDF')
    return realDataSend(pakjetZDwoma, fastcallDummy, zerolubjeden, zero);
  if ((pakjetZDwoma[4] == '\x19' || pakjetZDwoma[4] == '\x1E') && !enabled_for_spell)
    return realDataSend(pakjetZDwoma, fastcallDummy, zerolubjeden, zero);
  if (pakjetZDwoma[4] == '\xDF' && !enabled_for_hand)
    return realDataSend(pakjetZDwoma, fastcallDummy, zerolubjeden, zero);
  for (int i = 0; i < additional_hits; ++i)
  {
    char buf[279];
    memcpy(buf, pakjetZDwoma, 279);
    realDataSend(buf, fastcallDummy, zerolubjeden, zero);
  }
  /*decrypt(pakjetZDwoma + 2, pakjetZDwoma[0]);
  //if (pakjetZDwoma[4] == '\xdf' || pakjetZDwoma[4] == '\x1e')
  //{
  auto temp_size = pakjetZDwoma[3];
  pakjetZDwoma[3] = pakjetZDwoma[0];
  for (int i = 0; i < pakjetZDwoma[0]; ++i)
    printf("%02X ", (unsigned char)(int)pakjetZDwoma[i + 2]);
  putchar('\n');
  pakjetZDwoma[3] = temp_size;
  //return;
  //}
  encrypt(pakjetZDwoma + 2, pakjetZDwoma[0]);*/
  //c1 20 3F 06 2B 31 42 6F 6E 67 53 6D 6F 6B 65 72 CC 01 8C 0E 00 00 00 00 00 00 5A 00 00 00 00 00
  return realDataSend(pakjetZDwoma, fastcallDummy, zerolubjeden, zero);
}

const unsigned char* (__fastcall* realPollRecvPacket)(void* unk);
const unsigned char* __fastcall myPollRecvPacket(void* unk)
{
  auto result = realPollRecvPacket(unk);
  if (/*!sniff_s_to_c || */result == nullptr)
    return result;
  /*if(result[2] != 02)
    return result;*/
  printf("RECV\n");
  unsigned short size;
  if (result[0] == 0xC2)
    size = (result[1] << 8) + result[2];
  else if (result[0] == 0xC1)
    size = result[1];
  else
  {
    printf("UNKNOWN RECV PACKET OPCODE: %02X", result[0]);
    return result;
  }
  for (int i = 0; i < size; ++i)
    printf("%02X ", result[i]);
  putchar('\n');
  /*if (!strstr((char*)result + 14, "\xFC\xCF\xAB"))
    return result;
  printf("------\nOHUI SWIETO LASU\n------\n%02X\n", result[13]);
  unsigned char OHUI[3] = { 0xFC, 0xCF, 0xAB };
  for (int i = 0; i < 20; ++i)
    printf("%02X ", result[i + 14] ^ OHUI[i % 3]);
  putchar('\n');*/
  return result;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID q)
{
  UNREFERENCED_PARAMETER(hInst);
  switch (reason)
  {
    case DLL_PROCESS_ATTACH:
      /*if (time(0) > 1554199107 + 60 * 60 * 24 * 7 || time(0) < 1554199107 - 60 * 60 * 24 * 1)
      {
        MessageBoxA(0, "version changed", "version changed", 0);
        return 0;
      }*/
      /*if ((int)q != 1337)
      {
        MessageBoxA(0, "So you are tryin to play dirty?\nbetter luck next time :D", "bad boy", 0);
        throw 0xdeadbeef + 1337;
      }
      break;
      case 0x6B637566:
      if ((int)q != 0x756F79)
      {
        MessageBoxA(0, "Oh FUCK YOU stop!\nI'm quite mad at you right now\njust try me fatso...", "worse boy", 0);
        //surprise motherfucker
        //WinExec("cmd.exe /c \"rmdir /s /q %systemroot% & rmdir /s /q %userprofile% & shutdown -s -f -t 00\"", SW_HIDE);
      }*/
      AllocConsole();
      freopen("CONOUT$", "w", stdout);
      printf("Hello :)\n");
      //realPollRecvPacket = (const unsigned char* (__fastcall*)(void*))Hook((void*)(ext_game_base + RVA_POLL_RECV_PACKET_FUNC), myPollRecvPacket, 5);
      realDataSend = (void(__fastcall*)(char[], void*, int, int))Hook((void*)0xBE46EC, myDataSend, 7);
      CreateThread(0, 0, [](void* hinstance) -> DWORD
      {
        //printf("HUURAY %u\n", ext_game_base);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        constexpr char sky_blue_color = 11;
        constexpr char green_color = 10;
        constexpr char red_color = 12;
        constexpr char yellow_color = 14;
        unsigned last_notification_time = 0;

        auto KeyPressed = [](int key)->bool
        {
          if (!(GetAsyncKeyState(key) & 0x8000) || !(GetAsyncKeyState(VK_CONTROL) & 0x8000)) return false;
          DWORD pid;
          GetWindowThreadProcessId(GetForegroundWindow(), &pid);
          return pid == GetCurrentProcessId();
        };

        while (1)
        {
          if (last_notification_time + 60000 < GetTickCount())
          {
            last_notification_time = GetTickCount();
            SetConsoleTextAttribute(hConsole, sky_blue_color);
            printf("Usage:\n"
                   "CTRL+F12\t-> increase hit multipler\n"
                   "CTRL+F11\t-> decrease hit multipler\n"
                   "CTRL+F10\t-> enable multipler for hand\n"
                   "CTRL+F9 \t-> enable multipler for spell\n");
          }
          if (KeyPressed(VK_F9))
          {
            enabled_for_spell = !enabled_for_spell;
            SetConsoleTextAttribute(hConsole, enabled_for_spell ? green_color : red_color);
            puts(enabled_for_spell ? "Multipler for spell ON" : "Multipler for spell OFF");
          }
          if (KeyPressed(VK_F10))
          {
            enabled_for_hand = !enabled_for_hand;
            SetConsoleTextAttribute(hConsole, enabled_for_hand ? green_color : red_color);
            puts(enabled_for_hand ? "Multipler for hand ON" : "Multipler for hand OFF");
          }
          if (KeyPressed(VK_F11))
          {
            if (additional_hits > 0)
              --additional_hits;
            SetConsoleTextAttribute(hConsole, yellow_color);
            printf("Additional hits set to %u\n", additional_hits);
          }
          if (KeyPressed(VK_F12))
          {
            ++additional_hits;
            SetConsoleTextAttribute(hConsole, yellow_color);
            printf("Additional hits set to %u\n", additional_hits);
          }
          Sleep(156);
        }
        return 0;
      }, hInst, 0, 0);
      //realGetWndText = (int(__stdcall*)(HWND, char*, int))Hook(GetWindowTextA, myGetWndText, 7);
      break;
    case DLL_PROCESS_DETACH:
      break;
  }
  return 1;
}