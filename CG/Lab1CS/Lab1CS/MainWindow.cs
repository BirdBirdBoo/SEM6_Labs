using OpenTK.Graphics.ES30;
using OpenTK.Mathematics;
using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using PrimitiveType = OpenTK.Graphics.OpenGL.PrimitiveType;
using GL10 = OpenTK.Graphics.OpenGL.GL;

namespace Lab1CS;

public class MainWindow : GameWindow
{
    public MainWindow(int width, int height, string title)
        : base(new GameWindowSettings { RenderFrequency = 200, UpdateFrequency = 200 }, new NativeWindowSettings
        {
            Size = new Vector2i(width, height),
            Title = title
        })
    {
    }

    protected override void OnLoad()
    {
        base.OnLoad();

        GL.ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    }

    protected override void OnResize(ResizeEventArgs e)
    {
        base.OnResize(e);

        GL.Viewport(0, 0, e.Width, e.Height);
    }

    protected override void OnUpdateFrame(FrameEventArgs args)
    {
        base.OnUpdateFrame(args);

        GL.Clear(ClearBufferMask.ColorBufferBit);

        GL10.Begin(PrimitiveType.Lines);

        GL10.Color3(Color.Red);

        GL10.Vertex2(200, 200);
        GL10.Vertex2(400, 400);

        GL10.End();

        SwapBuffers();
    }
}