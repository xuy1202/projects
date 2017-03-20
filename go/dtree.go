package main

import "fmt"
import "os"
import "io"
import "strings"
import "bufio"
import "regexp"
//import "sort"
import "time"
import "runtime"

var INIT_TAG_MAP_SIZE uint64 = 0


type TagMap struct {
    IdxToTag map[uint64]string
    TagToIdx map[string]uint64
}

func NewTagMap () TagMap {
    tm := TagMap{
        make(map[uint64]string, INIT_TAG_MAP_SIZE),
        make(map[string]uint64, INIT_TAG_MAP_SIZE),
    }
    tm.TagToIdx[""] = 0
    tm.IdxToTag[0]  = ""
    return tm
}

func (m TagMap) GetIdx(tag string) uint64 {
    idx, ok := m.TagToIdx[tag]
    if ok{
        return idx
    }
    return 0
}

func (m TagMap) GetTag(idx uint64) string {
    tag, ok := m.IdxToTag[idx]
    if ok {
        return tag
    }
    return ""
}

func (m TagMap) GetSetTag(tag string) uint64 {
    var idx uint64
    var ok  bool
    idx, ok = m.TagToIdx[tag]
    if(! ok){
        idx = uint64(len(m.TagToIdx))
        m.TagToIdx[tag] = idx
        m.IdxToTag[idx] = tag
    }
    return idx
}

var GlobalTagMap TagMap = NewTagMap()


type Node struct {
    tagidx uint64
    father *Node
    sons   map[uint64]*Node
}

type Dtree struct {
    root *Node
}

type DtreeMap struct {
    SubDtrees map[string]*Dtree
}

func (dmap DtreeMap) RegisterSubTree(tag string){

}

func NewDtree () Dtree {
    var dtree Dtree
    dtree.root = &Node{
        GlobalTagMap.GetSetTag(""),
        nil,
        make(map[uint64]*Node),
    }
    return dtree
}


func (dtree Dtree) Feed(tags ...string){
    curnode := dtree.root;
    for _, tag := range tags{
        idx := GlobalTagMap.GetSetTag(tag)
        var n *Node
        n, ok := curnode.sons[idx]
        if ok {
            curnode = n
        }else{
            newnode := &Node{
                idx,
                curnode,
                make(map[uint64]*Node),
            }
            curnode.sons[idx] = newnode
            curnode = newnode
        }
    }
}

func (dtree Dtree) ParseFromFile(file string, reversed bool) bool {
    f, err := os.OpenFile(file, os.O_RDONLY, 0660)
    if err != nil{
        fmt.Println("ParseFromFile Error: ")
        return false
    }
    reader := bufio.NewReader(f)
    for {
        buf, err := reader.ReadString('\n')
        if err != nil{
            if err == io.EOF{
                break
            }
            return false
        }
        line := strings.Trim(buf, " \n")
        item := strings.Split(line, ".")
        if reversed {
            for i, j:=0,len(item)-1; i<j; i,j = i+1,j-1 {
                item[i], item[j] = item[j] , item[i]
            }
            //sort.Reverse(sort.StringSlice(item[:]))
        }
        dtree.Feed(item...)
    }
    return true
}


type WalkerStat struct {
    // stat value
    IsLeaf       bool
    PastNodes []*Node
    // feedback control
    ShouldStop   bool
}

type WalkerBase struct {
}

func (walker WalkerBase) SavePath(stat *WalkerStat) string {
    buf := make([]byte, 0)
    st := false
    for idx, i := range stat.PastNodes {
        if idx == 0 {
            continue
        }
        if ! st {
            st = true
        }else{
            buf = append(buf, '.')
        }
        buf = append(buf, GlobalTagMap.GetTag(i.tagidx)...)
    }
    return string(buf)
}


type PreOrderWalker interface {
    Process(stat *WalkerStat) bool
    SavePath(stat *WalkerStat) string
}


func (node *Node) preOrderWalk(walker PreOrderWalker, stat *WalkerStat){
    stat.PastNodes = append(stat.PastNodes, node)
    stat.IsLeaf    = (len(node.sons) == 0)
    defer func(){
        stat.PastNodes = stat.PastNodes[:len(stat.PastNodes)-1]
    }()

    if(walker.Process(stat)){
        s := walker.SavePath(stat)
        fmt.Println("R: " + s)
    }
    for _, node := range node.sons {
        node.preOrderWalk(walker, stat)
    }
}


func (dtree Dtree) PreOrderWalk(walker PreOrderWalker, stat *WalkerStat){
    dtree.root.preOrderWalk(walker, stat)
}


func (node *Node) show(layer int) {
    for i:=0; i<layer; i++ {
        fmt.Printf("  ")
    }
    fmt.Printf("%v:%v\n", GlobalTagMap.GetTag(node.tagidx), node.tagidx)
    for _, v := range node.sons {
        v.show(layer + 1)
    }
}

func (dtree Dtree) Show () {
    dtree.root.show(0)
}


// Impl
type LevelWalker struct {
    WalkerBase
    LeafOnly bool
    MaxLevel int
    MinLevel int
}


func (walker LevelWalker) Process(stat *WalkerStat) bool {
    if walker.LeafOnly && !stat.IsLeaf {
        return false
    }
    level := len(stat.PastNodes) - 1
    if level < walker.MinLevel{
        return false
    }
    if level > walker.MaxLevel{
        return false
    }
    return true
}

type RegexWalker struct {
    WalkerBase
    LeafOnly bool
    Pattern *regexp.Regexp
}

func (walker RegexWalker) Process(stat *WalkerStat) bool {
    if walker.LeafOnly && !stat.IsLeaf {
        return false
    }
    s := walker.SavePath(stat)
    ok := walker.Pattern.Match([]byte(s))
    if ok {
        fmt.Println("Y: " + s)
    }else{
        fmt.Println("N: " + s)
    }
    return false
}

type LevelRegexWalker struct{
    LeafOnly bool
    WalkerBase
    LevelWalker
    RegexWalker
}


func (walker LevelRegexWalker) Process(stat *WalkerStat) bool {
    ret := walker.LevelWalker.Process(stat)
    ret = ret && walker.RegexWalker.Process(stat)
    return ret
}

func MakeChanPair() (<-chan int, chan<- int) {
    ch := make(chan int)
    r := (<-chan int)(ch)
    w := (chan<- int)(ch)
    return r, w
}

type SubDtreeRpcCall struct{
    uuid uint64
    vals string
}

type SubDtreeRpcRsp struct{
    uuid uint64
    vals string
}

func TagedSubDtree(ch chan SubDtreeRpcCall) {
    dtree := NewDtree()
    timeout := make(chan bool, 1)
    go func(){
        for{
            time.Sleep(1 * time.Second)
            timeout <- true
        }
    }()
    for{
        select{
            case call := <-ch:
                if call.name == "feed"{
                    item := strings.Split(call.parm, ".")
                    fmt.Println(item)
                    dtree.Feed(item...)
                }else if call.name == "query"{
                    fmt.Println("Qeury: ", call.name)
                }else{
                    fmt.Println("Unknown call name")
                }
            case <-timeout:
                fmt.Println("Timeout")
        }
    }
    //dtree.ParseFromFile("/home/dev/data/domain.biz", false)
    //dtree.Show()
}


func main(){
    runtime.GOMAXPROCS(2)

    r, w := MakeChanPair()
    fmt.Println(r, w)

    dtree := NewDtree()
    st := time.Now()
    ret := dtree.ParseFromFile("/data/xuyang-pd/data/ddata/all_dns.20140912-10.diss2.succ.info", false)
    fmt.Println(ret)
    fmt.Println(time.Now().Sub(st))

    //ch := make(chan SubDtreeRpcCall)
    //go TagedSubDtree(ch)
    //time.Sleep(5 * time.Second)
    //ch <- SubDtreeRpcCall{"feed", "com.baidu.www"}
    //ch <- SubDtreeRpcCall{"query", "com.baidu.www"}
    //ch <- SubDtreeRpcCall{"asdf", "com.baidu.www"}

    // //stat := &WalkerStat{false, make([]*Node, 0), false}

    // //Lwalker := LevelWalker{}
    // //Lwalker.LeafOnly = true
    // //Lwalker.MinLevel = 4
    // //Lwalker.MaxLevel = 4
    // //dtree.PreOrderWalk(Lwalker, stat)

    // p, e := regexp.Compile(".*\\.baidu.*")
    // if e != nil{
    //     fmt.Println("regext compile error")
    // }else{
    //     //Rwalker := RegexWalker{}
    //     //Rwalker.LeafOnly = true
    //     //Rwalker.Pattern = p
    //     //dtree.PreOrderWalk(Rwalker, stat)

    //     LRWalker := LevelRegexWalker{}
    //     LRWalker.LeafOnly = true
    //     LRWalker.Pattern  = p
    //     LRWalker.MinLevel = 3
    //     LRWalker.MaxLevel = 3
    //     //dtree.PreOrderWalk(LRWalker, stat)
    // }
    time.Sleep(60 * time.Second)
}



