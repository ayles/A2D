import GLKit

class ViewController: GLKViewController {

    private var context: EAGLContext?
    private var touches: [UITouch] = []
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        context = EAGLContext(api: .openGLES3)
        EAGLContext.setCurrent(context)
        
        if let view = self.view as? GLKView, let context = context {
            view.context = context
            delegate = self
        }

        a2d_initialize()
    }
    
    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        a2d_resolution_changed(Int32(view.bounds.size.width), Int32(view.bounds.size.height), Int32(view.drawableWidth), Int32(view.drawableHeight))
        a2d_step()
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        self.touches.append(contentsOf: touches)
        sendTouches()
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        sendTouches()
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        self.touches.removeAll(where: { touches.contains($0) })
        sendTouches()
    }
    
    func sendTouches() {
        var index = 0
        touches.forEach { touch in
            let location = touch.location(in: self.view)
            a2d_touch_event(Int32(touches.count), Int32(index), Int32(touch.phase.rawValue), Float(location.x), Float(location.y))
            index += 1
        }
    }
    
    deinit {
        a2d_uninitialize()
    }

}

extension ViewController: GLKViewControllerDelegate {
    func glkViewControllerUpdate(_ controller: GLKViewController) {

    }
}
