const __vite__mapDeps=(i,m=__vite__mapDeps,d=(m.f||(m.f=["../nodes/0.DKV6a5vw.js","../chunks/disclose-version.CeDJfB-k.js","../chunks/index-client.sqKlkaDH.js","../chunks/Icon.BKMukHSk.js","../chunks/props.BnARJ_gJ.js","../chunks/legacy.r9OWb04V.js","../assets/Icon.BIaSm-mh.css","../assets/0.BA0pOvfq.css","../nodes/1.Bhy9N-Jh.js","../chunks/entry.Cn2LZ7ML.js","../nodes/2.CG7hXUsp.js","../chunks/preload-helper.DruTbtUk.js","../chunks/input.BFGgkzsG.js","../assets/2.CnS1cr6S.css","../nodes/3.vE8Vfkzn.js"])))=>i.map(i=>d[i]);
var q=e=>{throw TypeError(e)};var G=(e,t,r)=>t.has(e)||q("Cannot "+r);var o=(e,t,r)=>(G(e,t,"read from private field"),r?r.call(e):t.get(e)),k=(e,t,r)=>t.has(e)?q("Cannot add the same private member more than once"):t instanceof WeakSet?t.add(e):t.set(e,r),w=(e,t,r,n)=>(G(e,t,"write to private field"),n?n.call(e,r):t.set(e,r),r);import{c as j,b as A,_ as E}from"../chunks/preload-helper.DruTbtUk.js";import{j as m,a3 as Q,H as O,a9 as U,aa as W,a7 as X,p as Z,a as p,b as $,ab as tt,ac as et,m as R,n as rt,ad as L,v as st,o as at,q as ot,t as nt,k as C}from"../chunks/index-client.sqKlkaDH.js";import{h as ct,m as it,u as ut,a as b,t as H,c as D,b as dt,s as lt}from"../chunks/disclose-version.CeDJfB-k.js";import{p as I,a as mt,i as T}from"../chunks/props.BnARJ_gJ.js";function _t(e){return class extends ft{constructor(t){super({component:e,...t})}}}var _,i;class ft{constructor(t){k(this,_);k(this,i);var v;var r=new Map,n=(s,a)=>{var f=X(a);return r.set(s,f),f};const d=new Proxy({...t.props||{},$$events:{}},{get(s,a){return m(r.get(a)??n(a,Reflect.get(s,a)))},has(s,a){return a===Q?!0:(m(r.get(a)??n(a,Reflect.get(s,a))),Reflect.has(s,a))},set(s,a,f){return O(r.get(a)??n(a,f),f),Reflect.set(s,a,f)}});w(this,i,(t.hydrate?ct:it)(t.component,{target:t.target,anchor:t.anchor,props:d,context:t.context,intro:t.intro??!1,recover:t.recover})),(!((v=t==null?void 0:t.props)!=null&&v.$$host)||t.sync===!1)&&U(),w(this,_,d.$$events);for(const s of Object.keys(o(this,i)))s==="$set"||s==="$destroy"||s==="$on"||W(this,s,{get(){return o(this,i)[s]},set(a){o(this,i)[s]=a},enumerable:!0});o(this,i).$set=s=>{Object.assign(d,s)},o(this,i).$destroy=()=>{ut(o(this,i))}}$set(t){o(this,i).$set(t)}$on(t,r){o(this,_)[t]=o(this,_)[t]||[];const n=(...d)=>r.call(this,...d);return o(this,_)[t].push(n),()=>{o(this,_)[t]=o(this,_)[t].filter(d=>d!==n)}}$destroy(){o(this,i).$destroy()}}_=new WeakMap,i=new WeakMap;const wt={};var ht=H('<div id="svelte-announcer" aria-live="assertive" aria-atomic="true" style="position: absolute; left: 0; top: 0; clip: rect(0 0 0 0); clip-path: inset(50%); overflow: hidden; white-space: nowrap; width: 1px; height: 1px"><!></div>'),vt=H("<!> <!>",1);function gt(e,t){Z(t,!0);let r=I(t,"components",23,()=>[]),n=I(t,"data_0",3,null),d=I(t,"data_1",3,null);p(()=>t.stores.page.set(t.page)),$(()=>{t.stores,t.page,t.constructors,r(),t.form,n(),d(),t.stores.page.notify()});let v=L(!1),s=L(!1),a=L(null);tt(()=>{const c=t.stores.page.subscribe(()=>{m(v)&&(O(s,!0),et().then(()=>{O(a,mt(document.title||"untitled page"))}))});return O(v,!0),c});const f=C(()=>t.constructors[1]);var V=vt(),M=R(V);{var Y=c=>{var l=D();const y=C(()=>t.constructors[0]);var P=R(l);j(P,()=>m(y),(h,g)=>{A(g(h,{get data(){return n()},get form(){return t.form},children:(u,Pt)=>{var S=D(),J=R(S);j(J,()=>m(f),(K,N)=>{A(N(K,{get data(){return d()},get form(){return t.form}}),x=>r()[1]=x,()=>{var x;return(x=r())==null?void 0:x[1]})}),b(u,S)},$$slots:{default:!0}}),u=>r()[0]=u,()=>{var u;return(u=r())==null?void 0:u[0]})}),b(c,l)},z=c=>{var l=D();const y=C(()=>t.constructors[0]);var P=R(l);j(P,()=>m(y),(h,g)=>{A(g(h,{get data(){return n()},get form(){return t.form}}),u=>r()[0]=u,()=>{var u;return(u=r())==null?void 0:u[0]})}),b(c,l)};T(M,c=>{t.constructors[1]?c(Y):c(z,!1)})}var B=st(M,2);{var F=c=>{var l=ht(),y=at(l);{var P=h=>{var g=dt();nt(()=>lt(g,m(a))),b(h,g)};T(y,h=>{m(s)&&h(P)})}ot(l),b(c,l)};T(B,c=>{m(v)&&c(F)})}b(e,V),rt()}const jt=_t(gt),At=[()=>E(()=>import("../nodes/0.DKV6a5vw.js"),__vite__mapDeps([0,1,2,3,4,5,6,7]),import.meta.url),()=>E(()=>import("../nodes/1.Bhy9N-Jh.js"),__vite__mapDeps([8,1,2,5,9]),import.meta.url),()=>E(()=>import("../nodes/2.CG7hXUsp.js"),__vite__mapDeps([10,1,2,5,3,4,6,11,12,13]),import.meta.url),()=>E(()=>import("../nodes/3.vE8Vfkzn.js"),__vite__mapDeps([14,1,2,3,4,5,6,12]),import.meta.url)],Lt=[],Ct={"/":[2],"/pokemon":[3]},bt={handleError:({error:e})=>{console.error(e)},reroute:()=>{},transport:{}},yt=Object.fromEntries(Object.entries(bt.transport).map(([e,t])=>[e,t.decode])),Dt=!1,It=(e,t)=>yt[e](t);export{It as decode,yt as decoders,Ct as dictionary,Dt as hash,bt as hooks,wt as matchers,At as nodes,jt as root,Lt as server_loads};
