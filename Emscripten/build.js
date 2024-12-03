addToLibrary({
    JS_GenGUID: (stringPtr) => {
        // https://stackoverflow.com/questions/105034/how-do-i-create-a-guid-uuid
        let result = "10000000-1000-4000-8000-100000000000".replace(/[018]/g, c =>
            (+c ^ crypto.getRandomValues(new Uint8Array(1))[0] & 15 >> +c / 4).toString(16)
        );
        for (let i = 0, size = result.length; i < size; ++i) {
            Module.setValue(stringPtr + i, result.charCodeAt(i), "i8");
        }
    }
  });