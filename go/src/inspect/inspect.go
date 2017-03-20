package inspect

import "fmt"
import "reflect"



func  ShowField(val interface{}){
    t := reflect.TypeOf(val)
    v := reflect.ValueOf(val)
    for i:=0; i<v.NumField(); i++ {
         f := v.Field(i)
         if(f.CanInterface()){
             fmt.Println(t.Field(i).Name, f.Interface())
         }
    }
}


func Inspect(val interface{}){

    var inspect_basic func (name string, val interface{}, layer int)
    suojin := func (layer int) {
        for i:=0; i<layer; i++{
            if i == layer-1{
                fmt.Print(" |--")
            }else{
                fmt.Print(" |  ")
            }
        }
    }

    inspect_struct := func (val interface{}, layer int){
        t := reflect.TypeOf(val)
        v := reflect.ValueOf(val)
        for i:=0; i<v.NumField(); i++ {
             f := v.Field(i)
             if(f.CanInterface()){
                 inspect_basic(t.Field(i).Name, f.Interface(), layer)
             }
        }
        for i:= 0; i<v.NumMethod(); i++ {
            m := t.Method(i)
            suojin(layer)
            fmt.Printf("%v = %v \n", m.Name, m.Type)
        }
    }

    inspect_basic = func (name string, val interface{}, layer int){
        t := reflect.TypeOf(val)
        v := reflect.ValueOf(val)
        suojin(layer)
        switch t.Kind() {
            case reflect.Uintptr:
                fallthrough
            case reflect.UnsafePointer:
                fallthrough
            case reflect.Ptr:
                fallthrough
            case reflect.Bool:
                fallthrough
            case reflect.Int:
                fallthrough
            case reflect.Int8:
                fallthrough
            case reflect.Int16:
                fallthrough
            case reflect.Int32:
                fallthrough
            case reflect.Int64:
                fallthrough
            case reflect.Uint:
                fallthrough
            case reflect.Uint8:
                fallthrough
            case reflect.Uint16:
                fallthrough
            case reflect.Uint32:
                fallthrough
            case reflect.Uint64:
                fallthrough
            case reflect.Float32:
                fallthrough
            case reflect.Float64:
                fallthrough
            case reflect.Complex64:
                fallthrough
            case reflect.Complex128:
                fallthrough
            case reflect.Array:
                fallthrough
            case reflect.Chan:
                fallthrough
            case reflect.Func:
                fallthrough
            case reflect.Interface:
                fallthrough
            case reflect.Map:
                fallthrough
            case reflect.Slice:
                fallthrough
            case reflect.String:
                if len(name) > 0{
                    fmt.Printf("%v = %v(%v)\n", name, v.Type(), val)
                }else{
                    fmt.Printf("%v(%v)\n", v.Type(), val)
                }
            case reflect.Struct:
                fmt.Printf("%v<%v>:\n", t.Name(), t.PkgPath())
                inspect_struct(val, layer+1)
            default:
                fmt.Println("Unknown: ", val)
        }
    }

    inspect_basic("", val, 0)
}


func main(){
    i := 3
    var ii *int
    ii = &i
    fmt.Println("\n>>> Inspect Int:")
    Inspect(i)
    fmt.Println("\n>>> Inspect Int Ptr:")
    Inspect(ii)
    fmt.Println("\n>>> Inspect Int:")
    Inspect(*ii)
    fmt.Println("\n>>> Inspect Array:")
    Inspect([]int{1,2,3})
    fmt.Println("\n>>> Inspect Slice:")
    Inspect(make([]int, 3))
    fmt.Println("\n>>> Inspect Chan:")
    Inspect(make(chan int))
    fmt.Println("\n>>> Inspect Map:")
    Inspect(make(map[string]int, 3))
    fmt.Println("\n>>> Inspect Func:")
    Inspect(Func0)

    fmt.Println("\n>>> Inspect Obj1:")
    Inspect(Obj1{"attr1", true})

    fmt.Println("\n>>> Inspect Obj2:")
    o2 := Obj2{
        Obj1{"attr1_begin_1", true},
        Obj0{"attr1_begin_2"},
        4,
        64,
    }
    Inspect(o2)
    fmt.Println("\n>>> Inspect Obj2(changed):")
    // NO: o2.Attr1 = ""
    //    ambiguous selector o2.Attr1
    o2.Obj0.Attr1 = "attr1_changed"
    Inspect(o2)

    fmt.Println("\n>>> Inspect Obj3:")
    o3 := Obj3{
        Obj1{"attr1_inner_begin_1", false},
        Obj0{"attr1_inner_begin_2"},
        "attr1_outer_begin",
        0,
    }
    Inspect(o3)
    fmt.Println("\n>>> Inspect Obj3(changed):")
    o3.Attr1 = "attr1_changed"
    Inspect(o3)

    fmt.Println("\n>>> Inspect Overlaped Obj4:")
    Inspect(Obj4{})
}



