using System;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Animation;
using System.Windows.Media.Media3D;

namespace Lab8;

public partial class MainWindow
{
    private const double MoveSpeed = 8;
    private const double ScaleSpeed = 2;
    
    private bool _isEditing;
    private bool _isAnimationRunning = true;
    private bool _isAnimationPausedByEdit;
    private EditMode _editMode = EditMode.None;
    private Point _mouseEditOrigin = new(double.NaN, double.NaN);

    private bool IsEditOriginSet => !double.IsNaN(_mouseEditOrigin.X);

    private double ViewportWidth => Viewport.RenderSize.Width;
    private double ViewportHeight => Viewport.RenderSize.Width;

    private Storyboard CubeRotationStoryboard => GetWindowResource<Storyboard>();
    private ScaleTransform3D CubeScaleTransform => GetWindowResource<ScaleTransform3D>();
    private TranslateTransform3D CubeTranslateTransform => GetWindowResource<TranslateTransform3D>();

    public MainWindow()
    {
        InitializeComponent();

        KeyUp += OnCubeKeyUp;
        CubeRotationStoryboard.Begin();
    }

    private void BeginEditing(EditMode mode)
    {
        _editMode = mode;
        Console.WriteLine($"Beginning editing, mode: {_editMode}, isAnimation: {_isAnimationRunning}");
        _isEditing = true;

        if (_isAnimationRunning)
        {
            ToggleAnimation();
            _isAnimationPausedByEdit = true;
        }

        StartMouseCapture();
    }

    private void StopEditing()
    {
        Console.WriteLine($"Stopping editing, mode: {_editMode}, animationPausedByEdit: {_isAnimationPausedByEdit}");
        _editMode = EditMode.None;
        _isEditing = false;
        _mouseEditOrigin = new Point(double.NaN, double.NaN);

        if (_isAnimationPausedByEdit)
        {
            ToggleAnimation();
        }

        _isAnimationPausedByEdit = false;

        StopMouseCapture();
    }

    private void StartMouseCapture()
    {
        MouseUp += OnCubeMouseUp;
        MouseMove += OnCubeMouseMove;
        Mouse.Capture(this, CaptureMode.SubTree);
    }

    private void StopMouseCapture()
    {
        MouseUp -= OnCubeMouseUp;
        MouseMove -= OnCubeMouseMove;
    }

    private void OnCubeMouseMove(object sender, MouseEventArgs e)
    {
        if (!_isEditing)
        {
            return;
        }
        
        var mousePosition = e.GetPosition(this);
        
        if (!IsEditOriginSet)
        {
            _mouseEditOrigin = mousePosition;
            return;
        }

        var mouseDelta = mousePosition - _mouseEditOrigin;
        
        switch (_editMode)
        {
            case EditMode.Scale:
                var scale = 0.5 + new Vector(mouseDelta.X / ViewportWidth, mouseDelta.Y / ViewportHeight).Length * ScaleSpeed;

                CubeScaleTransform.ScaleX = scale;
                CubeScaleTransform.ScaleY = scale;
                CubeScaleTransform.ScaleZ = scale;
                break;
            case EditMode.Move:
                CubeTranslateTransform.OffsetY = -mouseDelta.Y / ViewportHeight * MoveSpeed;
                CubeTranslateTransform.OffsetX = mouseDelta.X / ViewportWidth * MoveSpeed;
                
                break;
        }
    }

    private void OnCubeKeyUp(object sender, KeyEventArgs e)
    {
        if (e.Key is Key.Escape)
        {
            Close();
            
            return;
        }
        
        if (_isEditing)
        {
            return;
        }

        switch (e.Key)
        {
            case Key.M:
                BeginEditing(EditMode.Move);
                break;
            case Key.S:
                BeginEditing(EditMode.Scale);
                break;
            case Key.Space:
                ToggleAnimation();
                break;
        }
    }

    private void ToggleAnimation()
    {
        _isAnimationRunning = !_isAnimationRunning;
        if (!_isAnimationRunning)
        {
            CubeRotationStoryboard.Pause();
        }
        else
        {
            CubeRotationStoryboard.Resume();
        }
    }

    private void OnCubeMouseUp(object sender, MouseButtonEventArgs e)
    {
        switch (e.ChangedButton)
        {
            case MouseButton.Left:
            case MouseButton.Right:
                StopEditing();
                break;
        }
    }

    private TResource GetWindowResource<TResource>([CallerMemberName] string resource = null!)
    {
        return (TResource)Resources[resource];
    }

    enum EditMode
    {
        None,
        Move,
        Scale
    }
}