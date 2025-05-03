export default function NewsItem({title, content} : {title: string, content: any})
{
    return (
        <>
            <div className="border-solid border-red-400 border-2 p-5" >
                <h1 className="text-3xl font-bold text-slate-800">{title}</h1>
                <hr className="my-2" />
                {content}
            </div>
        </>
    )
}