namespace Lab1CS;

internal static class Program
{
    public static void Main()
    {
        using var mainWindow = new MainWindow(1280, 720, "Lab1CS");
        
        mainWindow.Run();
    }
}