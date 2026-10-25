## Editor Engine Class Diagram

```mermaid
classDiagram

namespace EngineApp {
	class EditorEngine
	class FEngineTimer
}

namespace Rendering {
	class URenderer
}

namespace EditorUI {
	class FEditorUiManager
}

namespace Core.Math {
	class FCamera
}

EditorEngine *-- FEngineTimer
EditorEngine *-- URenderer
EditorEngine *-- FEditorUiManager
EditorEngine *-- FCamera

```