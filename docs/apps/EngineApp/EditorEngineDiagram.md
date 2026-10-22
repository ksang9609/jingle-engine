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

EditorEngine *-- FEngineTimer
EditorEngine *-- URenderer
EditorEngine *-- FEditorUiManager

```