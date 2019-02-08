
open System
open System.Windows.Forms

let uuidstr = System.Guid.NewGuid().ToString("n")

Clipboard.SetText(uuidstr)
printfn "%s" uuidstr
